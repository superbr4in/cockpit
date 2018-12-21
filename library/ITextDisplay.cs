namespace Cockpit
{
    /*!
     *  Declares functions to display text.
     */
    public interface ITextDisplay
    {
        /*!
         *  Gets or sets the number of the topmost line visible of #Content.
         */
        int Line { get; set; }
        /*!
         *  Gets or sets the number of the leftmost column visible of #Content.
         */
        int Column { get; set; }

        /*!
         *  Gets or sets the lines of text to be displayed.
         */
        string[] Content { get; set; }

        /*!
         *  Gets or sets a brief text containing additional information.
         */
        string Status { get; set; }

        /*!
         *  Sets the topmost line and leftmost column visible of #Content.
         *  @param [in] line   Line number
         *  @param [in] column Column number
         */
        void SetPosition(int line, int column);
    }
}
