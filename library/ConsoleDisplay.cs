using System;
using System.Linq;

namespace Cockpit
{
    /*!
     *  Displays text on the application's console window with a status bar at the bottom.
     */
    public sealed class ConsoleDisplay : ITextDisplay
    {
        private static readonly ConsoleDisplay INSTANCE;

        static ConsoleDisplay()
        {
            INSTANCE = new ConsoleDisplay();
        }

        /*!
         *  Gets the singleton instance of this class.
         */
        public static ConsoleDisplay Instance
        {
            get { return INSTANCE; }
        }

        private const int STATUS_HEIGHT = 1;

        private bool _visible;

        private int _line, _column;

        private string[] _content;
        private int _contentWidth;

        private string _status;

        private ConsoleDisplay()
        {
            _visible = false;

            _line = 0;
            _column = 0;

            _content = null;
            _contentWidth = 0;

            _status = null;
        }

        /*! @copydoc ITextDisplay#Line
         */
        public int Line
        {
            get { return _line; }
            set { SetPosition(value, _column); }
        }
        /*! @copydoc ITextDisplay#Column
         */
        public int Column
        {
            get { return _column; }
            set { SetPosition(_line, value); }
        }

        /*! @copydoc ITextDisplay#Content
         */
        public string[] Content
        {
            get { return (string[])_content.Clone(); }
            set
            {
                _line = 0;
                _column = 0;

                if (value == null || value.Length == 0)
                {
                    _content = null;
                    _contentWidth = 0;
                }
                else
                {
                    _content = (string[])value.Clone();
                    _contentWidth = _content.Max(s => s.Length);
                }

                if (!_visible)
                    return;

                DrawContent();
            }
        }

        /*! @copydoc ITextDisplay#Status
         */
        public string Status
        {
            get { return _status; }
            set
            {
                if (string.IsNullOrEmpty(value))
                    _status = null;
                else
                    _status = value;

                if (!_visible)
                    return;

                DrawStatus();
            }
        }

        private static int StatusConsoleLine
        {
            get { return Console.WindowHeight - STATUS_HEIGHT; }
        }

        /*! @copydoc ITextDisplay#SetPosition(int,int)
         */
        public void SetPosition(int line, int column)
        {
            _line = ClampCoordinateValue(line, _content.Length - StatusConsoleLine);
            _column = ClampCoordinateValue(column, _contentWidth - Console.WindowWidth);

            if (!_visible)
                return;

            DrawContent();
        }

        /*!
         *  Makes #Content visible on the current console window.
         */
        public void Show()
        {
            Console.CursorVisible = false;

            _visible = true;

            DrawContent();
        }
        /*!
         *  Clears the displayed text to reuse the current console window.
         */
        public void Hide()
        {
            Console.SetCursorPosition(0, Console.WindowHeight - 1);
            ClearLine();

            Console.CursorVisible = true;
        }

        private void DrawContent()
        {
            for (var consoleLine = 0; consoleLine < StatusConsoleLine; consoleLine++)
            {
                Console.SetCursorPosition(0, consoleLine);
                ClearLine();

                var contentLine = _line + consoleLine;

                if (_content == null || contentLine >= _content.Length)
                    continue;

                var text = _content[contentLine];

                if (text.Length <= _column)
                    continue;

                text = text.Substring(_column);

                Console.Write(CapTextLength(text, Console.WindowWidth));
            }
        }

        private void DrawStatus()
        {
            Console.SetCursorPosition(0, StatusConsoleLine);
            ClearLine();

            if (_status == null)
                return;

            Console.Write(CapTextLength(_status, Console.WindowWidth * STATUS_HEIGHT));
        }

        private static void ClearLine()
        {
            var cursorLeft = Console.CursorLeft;

            Console.Write(new string(' ', Console.WindowWidth));
            Console.CursorLeft = cursorLeft;
        }

        private static int ClampCoordinateValue(int coordinateValue, int maxCoordinateValue)
        {
            if (coordinateValue > maxCoordinateValue)
                coordinateValue = maxCoordinateValue;

            if (coordinateValue <= 0)
                coordinateValue = 0;

            return coordinateValue;
        }

        private static string CapTextLength(string text, int maxTextLength)
        {
            if (text.Length > maxTextLength)
                return text.Substring(0, maxTextLength);

            return text;
        }
    }
}
