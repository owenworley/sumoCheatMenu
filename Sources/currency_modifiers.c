// Filename: currency_modifiers.c

#include "cheats.h"
#include "hid.h"

/********************************
*				*
*     Currency Modifiers	*
*				*
********************************/

static u32 quantity = 500000;

// Currency menu entry
void    currencyMenu(void) {
    new_spoiler("Currency");
        new_entry_managed("Increase Quantity +500,000", increaseCurrencyQuantity, INCREASECURRENCYQUANTITY, AUTO_DISABLE);
        new_separator();
        new_entry_managed("Poke Dollars          xCCCCCCC", maxMoney, MAXMONEY, EXECUTE_ONCE);
        new_entry_managed("Battle Points         xCCCCCCC", maxBP, MAXBP, EXECUTE_ONCE);
        new_entry_managed("Festival Coins        xCCCCCCC", maxCoins, MAXCOINS, EXECUTE_ONCE);
        new_entry_managed("Total Festival Coins  xCCCCCCC", totalCoins, TOTALCOINS, EXECUTE_ONCE);
        new_entry_managed("Thumbs Up             xCCCCCCC", totalThumbs, TOTALTHUMBS, EXECUTE_ONCE);
        new_line();
    exit_spoiler();
    updateCurrencyQuantity();
}

// Increases currency quantity by 500,000
void    increaseCurrencyQuantity(void) {
    if (quantity == 9999999)
        quantity = 0;
    if (quantity == 9500000)
        quantity = 9999999;
    if (quantity < 9500000)
        quantity += 500000;
    updateCurrencyQuantity();
}

// Updates currency quantity on menu
void    updateCurrencyQuantity(void) {
    char buf[9];

    xsprintf(buf, "x%-7d", quantity);
    replace_pattern("x*******", buf, MAXMONEY);
    replace_pattern("x*******", buf, TOTALCOINS);
    replace_pattern("x*******", buf, TOTALTHUMBS);
    if (quantity > 9999)
        replace_pattern("x*******", "x9999   ", MAXBP);
    else
        replace_pattern("x*******", buf, MAXBP);
    if (quantity > 999999)
        replace_pattern("x*******", "x999999 ", MAXCOINS);
    else
        replace_pattern("x*******", buf, MAXCOINS);
}


// Set Poke Dollars to 9,999,999
void	maxMoney(void) {
	WRITEU32(0x330D8FC0, quantity);
}

// Set current Festival Coins to 999,999
void	maxCoins(void) {
    WRITEU32(0x33124D58, (quantity > 999999) ? 0x000F423F : quantity);
}


// Set total Festival Coins to 2,000,000
void	totalCoins(void) {
    u32 current = READU32(0x33124D58);
    u32 total = quantity;
    u32 spent = total - current;
    WRITEU32(0x3313DCE8, spent);
	WRITEU32(0x33124D5C, total);
}


// Set total Thumbs Up for photos to 1,500,000
void	totalThumbs(void) {
	WRITEU32(0x33138B8C, quantity);
}


// Set Battle Points to 9,999
void	maxBP(void) {
	if (READU32(0x0067206C) != 0x00) {
		u32 offset;

		// offset = READU32(0x4 + READU32(0x24 + READU32(0x0067206C)));
        offset = READU32(0x0067206C);
        offset = READU32(0x00000024 + offset);
        offset = READU32(0x00000004 + offset);
        if (quantity > 9999)
            WRITEU16(0x000037B0 + offset, 0x0000270F);
        else
            WRITEU16(0x000037B0 + offset, quantity);
	}
}
