#include <utilitieslib/stdtypes.h>
#include "uiListView.h"
#include <utilitieslib/assert/assert.h>
#include <stddef.h>
#include <string.h>
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiUtil.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/ttFontUtil.h"
#include <utilitieslib/utils/mathutil.h>
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/utils/contextQsort.h>
#include "UI/uiInput.h"
#include "UI/uiClipper.h"
#include "UI/uiUtilGame.h"
#include "UI/uiTarget.h"
#include <utilitieslib/utils/Color.h>
#include "entity/entity.h"
#include "UI/uiCursor.h"
#include "win/win_init.h"
#include "winuser.h"
#include "UI/uiGame.h"

#include "UI/uiSupergroup.h"
#include "UI/uiEmail.h"
#include "UI/uiFriend.h"
#include "UI/uiLfg.h"
#include "UI/uiNet.h"

extern TTDrawContext* font_grp;

int gListViewDragging = 0;

#define DEFAULT_WD 100

int        gDivSuperName        = DEFAULT_WD;
int        gDivSuperMap        = DEFAULT_WD;
int        gDivSuperTitle        = DEFAULT_WD;
int        gDivEmailFrom        = DEFAULT_WD;
int        gDivEmailSubject    = DEFAULT_WD;
int        gDivFriendName        = DEFAULT_WD;
int        gDivLfgName            = DEFAULT_WD;
int        gDivLfgMap            = DEFAULT_WD;

typedef struct SavedColumn
{
    int                *width;
    char            *col_name;
    UIColumnHeader    *column;
}SavedColumn;

SavedColumn savedColumns[] = 
{
    { &gDivSuperName,        SG_MEMBER_NAME_COLUMN,    },
    { &gDivSuperMap,        SG_MEMBER_ZONE,            },
    { &gDivSuperTitle,        SG_MEMBER_TITLE_COLUMN,    },
    { &gDivEmailFrom,        EMAIL_FROM_COLUMN,        },
    { &gDivEmailSubject,    EMAIL_SUBJECT_COLUMN,    },
    { &gDivFriendName,        FRIEND_NAME_COLUMN,        },
    { &gDivLfgName,            LFG_NAME_COLUMN,        },
    { &gDivLfgMap,            LFG_MAP_COLUMN,            },
};


//--------------------------------------------------------------------------------------------------------------------
// Widet3Part
//--------------------------------------------------------------------------------------------------------------------


void Widget3PartDraw(Widget3Part* widget, float x, float y, float z, float scale, unsigned int size, int color)
{
    AtlasTex* part1;
    AtlasTex* part2;
    AtlasTex* part3;
    float midSize;
    float midScale;
    float* cursor;        // Which direction does the cursor move in? penX or penY?
    int cursorIncOffset;    // Look into this offset in the texbind to increment cursor.
    float penX = x, penY = y;

    if(!widget)
        return;

    // Make sure we have some reasonable input.
    assert(widget->part1 && widget->part2 && widget->part3);
    assert(widget->mode != WPM_NONE);

     part1 = atlasLoadTexture(widget->part1);
     part2 = atlasLoadTexture(widget->part2);
    part3 = atlasLoadTexture(widget->part3);

    // Decide which direction will be used for 
    if(widget->mode == WPM_HORIZONTAL)
    {
        cursor = &penX;
        cursorIncOffset = offsetof(AtlasTex, width);
        midSize = size - (part1->width+part3->width)*scale;
        midScale = midSize/part2->width;
    }
    else
    {
        cursor = &penY;
        cursorIncOffset = offsetof(AtlasTex, height);
        midSize = size - (part1->height+part3->height)*scale;
        midScale = midSize/part2->height;
    }

    display_sprite(part1, penX, penY, z, scale, scale, color|0xff);
    *cursor += *(int*)((char*)part1 + cursorIncOffset)*scale;

    if(widget->mode == WPM_HORIZONTAL)
    {
        display_sprite(part2, penX, penY, z, midScale, scale, color|0xff);
    }
    else
    {
        display_sprite(part2, penX, penY, z, scale, midScale, color|0xff);
    }
    *cursor += midSize;

    display_sprite(part3, penX, penY, z, scale, scale, color|0xff);
}

int Widget3PartHeight(Widget3Part* widget)
{
    int height;
    AtlasTex* part1;
    AtlasTex* part2;
    AtlasTex* part3;

    if(!widget)
        return 0;

    assert(widget->part1 && widget->part2 && widget->part3);
    assert(widget->mode != WPM_NONE);
    assert(widget->mode != WPM_VERTICAL);    // NOT SUPPORTED YET.

    part1 = atlasLoadTexture(widget->part1);
    part2 = atlasLoadTexture(widget->part2);
    part3 = atlasLoadTexture(widget->part3);

    height = MAX(part1->height, part2->height);
    height = MAX(height, part3->height);
    return height;
}

void Widget3PartGetWidths(Widget3Part* widget, unsigned int* part1Width, unsigned int* part2Width, unsigned int* part3Width)
{
    AtlasTex* part1;
    AtlasTex* part2;
    AtlasTex* part3;

    if(!widget)
        return;

    assert(widget->part1 && widget->part2 && widget->part3);
    assert(widget->mode != WPM_NONE);

    if(part1Width)
    {
        part1 = atlasLoadTexture(widget->part1);
        *part1Width = part1->width;
    }
    if(part2Width)
    {
        part2 = atlasLoadTexture(widget->part2);
        *part2Width = part2->width;
    }
    if(part3Width)
    {
        part3 = atlasLoadTexture(widget->part3);
        *part3Width = part3->width;
    }
}

Widget3Part ColumnHeaderParts =
{
    {
        "Headerbar_L.tga",
        "Headerbar_Mid.tga",
        "Headerbar_R.tga",
    },
    WPM_HORIZONTAL
};

Widget3Part ColumnSelectedHeaderParts =
{
    {
        "Headerbar_selected_L.tga",
        "Headerbar_selected_Mid.tga",
        "Headerbar_selected_R.tga",
    },
    WPM_HORIZONTAL
};

//--------------------------------------------------------------------------------------------------------------------
// UIColumnHeader 
//--------------------------------------------------------------------------------------------------------------------
#include <utilitieslib/components/earray.h>

UIColumnHeader* uiCHCreate(void)
{
    return calloc(1, sizeof(UIColumnHeader));
}

void uiCHDestroy(UIColumnHeader* column)
{
    if(!column)
        return;
    if(column->name)
        free(column->name);
    if(column->caption)
        free(column->caption);
}

UIListViewHeader* uiLVHCreate(void)
{
    UIListViewHeader* header = calloc(1, sizeof(UIListViewHeader));
    header->selectedColumn = -1;
    header->drawColor = CLR_WHITE;
    return header;
}

void uiLVHDestroy(UIListViewHeader* header)
{
    if(!header)
        return;
    if(header->columns)
    {
        int i;
        for( i = eaSize(&header->columns)-1; i >= 0; i-- )
        {
            SAFE_FREE(header->columns[i]->name);
            SAFE_FREE(header->columns[i]->caption);
        }
        eaDestroy(&header->columns);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// UIListViewHeader
//--------------------------------------------------------------------------------------------------------------------
#define HEADER_SEPARATOR_MARGINS 10
#define LIST_SELECTION_MIN_HEIGHT 23

// Min width is also limited by the width of the name string
/* Function uiLVHAddColumn()
 *    Add a column to the header, given the column's caption and a minimum width.
 *
 *    Note that the given minWidth only takes effect if it is larger than the
 *    width of the caption with the selection border on it.
 *
 *    WARNING!
 *        The currently selected font (font_group) effects how the wide
 *        a column can be.
 */
int uiLVHAddColumn(UIListViewHeader* header, char* name, char* caption, float minWidth)
{
    return uiLVHAddColumnEx(header, name, caption, minWidth, MAX( minWidth*2, str_wd(font_grp, 1.0, 1.0, caption)*2), 0 );
}

int uiLVHAddColumnEx(UIListViewHeader* header, char* name, char* caption, float minWidth, float maxWidth, int resizable)
{
    UIColumnHeader* column;
    unsigned int selectedHeaderPart1Width = 0;
    unsigned int selectedHeaderPart3Width = 0;

    if(!header || !name)
        return 0;

    column = uiCHCreate();
    column->name = strdup(name);
    column->caption = strdup(caption);
    column->minWidth = MAX( str_wd(font_grp, 1.f, 1.f, caption), MAX(10, minWidth));
    column->maxWidth = maxWidth;
    column->curWidth = str_wd(font_grp, 1.0, 1.0, column->caption);
    column->resizable = resizable;
    eaPush(&header->columns, column);

    if( resizable )
    {
        int i;
        for( i = 0; i < ARRAY_SIZE(savedColumns); i++ )
        {
            if( stricmp( name, savedColumns[i].col_name ) == 0 )
            {
                savedColumns[i].column = column;
                column->curWidth = *savedColumns[i].width;
            }
        }
    }

    return 1;
}

void uiLVHSetColumnWidth(UIListViewHeader* header, char* name, int width)
{
    int i;
    for( i = eaSize(&header->columns)-1; i >= 0; i-- )
    {
        if( stricmp( header->columns[i]->name, name ) == 0 )
            header->columns[i]->curWidth = width;
    }
}

void uiLVHSetColumnCaption(UIListViewHeader* header, char* name, char * caption)
{
    int i;
    for( i = eaSize(&header->columns)-1; i >= 0; i-- )
    {
        if( stricmp( header->columns[i]->name, name ) == 0 )
        {
            header->columns[i]->caption = realloc( header->columns[i]->caption, strlen(caption)+1);
            strcpy(header->columns[i]->caption, caption);
            header->columns[i]->curWidth = MAX((unsigned int)str_wd(font_grp, 1.0, 1.0, caption), header->minWidth);
            return;
        }
    }

}

/* Function uiLVHFindColumnIndex()
 *    Returns the index of the named column into UIListViewHeader::columns 
 *    or -1 if the column is not found.
 */
int uiLVHFindColumnIndex(UIListViewHeader* header, char* name)
{
    int i;
    if(!header || !name)
        return -1;

    for(i = 0; i < eaSize(&header->columns); i++)
    {
        if(stricmp(name, header->columns[i]->name) == 0)
            return i;
    }
    return -1;
}

/* Function uiLVHFinalizeSettings()
 *    This function should be called on a header after new columns are added
 *    to the header.
 */
int uiLVHFinalizeSettings(UIListViewHeader* header)
{
    uiLVHSetMinWidth(header, uiLVHGetMinDrawWidth(header));
    return 1;
}

/* Function uiLVHFinalizeSettings()
 *    Sets the current width of the header.
 *    The function only succeeds if the width if larger than or equal to the minimum width.
 */
int uiLVHSetWidth(UIListViewHeader* header, unsigned int width)
{
    if(!header || width < header->minWidth)
    {
        if( eaSize( &header->columns ) == 1 )
            header->minWidth = header->width = width;
        else
            width = header->minWidth;
    }
    header->width = width;
    return width;
}

/* Function uiLVHChangeMinWidth()
 *    Change the minWidth of the header.
 *    The function only succeeds if the width is larger than the minDrawWidth
 *    of the header.
 */
int uiLVHSetMinWidth(UIListViewHeader* header, unsigned int width)
{
    // The min width must be larger than the draw width.
    if( !header || ( width < uiLVHGetMinDrawWidth(header) ))
        return 0;

    header->minWidth = width;
    return 1;
}

/* Function uiLVHGetMinDrawWidth()
 *    Returns the minimum number of pixels required to draw the header.
 */
unsigned int uiLVHGetMinDrawWidth(UIListViewHeader* header)
{
    int i;
    unsigned int minWidth = 0;

    if(!header)
        return 0;

    // Account for all column minimum widths.
    for(i = 0; i < eaSize(&header->columns); i++)
    {
        minWidth += header->columns[i]->curWidth;
    }

    // Account for all separator + margin widths.
    {
        AtlasTex* separator;

        separator = atlasLoadTexture("Headerbar_VertDivider_Bottom.tga");
        minWidth += (separator->width + 2 * HEADER_SEPARATOR_MARGINS) * (eaSize(&header->columns) - 1);
    }

    // Account for header cap widths.
    {
        unsigned int selectedHeaderPart1Width = 0;
        unsigned int selectedHeaderPart3Width = 0;

        Widget3PartGetWidths(&ColumnHeaderParts, &selectedHeaderPart1Width, NULL, &selectedHeaderPart3Width);
        minWidth += selectedHeaderPart1Width + selectedHeaderPart3Width;
    }

    return minWidth;
}

/* Function uiLVHGetHeight()
 *    Get the height of the header when drawn.
 */
unsigned int uiLVHGetHeight(UIListViewHeader* header)
{
    if(!header)
        return 0;
    return Widget3PartHeight(&ColumnHeaderParts);
}

/* Function uiLVHSetDrawColor()
 *    Sets the draw color for the header when drawn in 0xRRGGBBAA format.
 *    The default draw color is full white.
 */
void uiLVHSetDrawColor(UIListViewHeader* header, int color)
{
    if(!header)
        return;
    header->drawColor = color;
}

/* Function uiCHIGetIterator()
 *    Sets up an iterator to go over each of the columns of in the specified
 *    header.
 */
int uiCHIGetIterator(UIListViewHeader* header, UIColumnHeaderIterator* iterator)
{
    if(!header || !iterator)
        return 0;

    memset(iterator, 0, sizeof(UIColumnHeaderIterator));
    iterator->header = header;
    iterator->columnIndex = -1;

    // How wide are the header caps?
    Widget3PartGetWidths(&ColumnHeaderParts, &iterator->headerPart1Width, NULL, &iterator->headerPart3Width);

    // How wide is a column separator?
    {
        AtlasTex* separator = atlasLoadTexture("Headerbar_VertDivider_Bottom.tga");
        iterator->separatorWidth = separator->width;
    }

    // Where does the first column start?
    iterator->columnStartOffset = iterator->headerPart1Width;

    // columnExtracSpacing is the number of pixels that should be added to each column so that
    // the header fills the entire specified width of the header.
    iterator->extraColumnSpacing = 0; //MAX(0, (int)(header->width - uiLVHGetMinDrawWidth(header)))/eaSize(&header->columns);
    return 1;
}

/* Function uiCHIGetNextColumn()
 *    Retrieves information associated with the next column when called.
 *
 *    The iterator can be used to implement a consistent way to determine the various graphical dimensions
 *    associated with the column.
 *
 *    Returns:
 *        1 - Column info retrieved successfully.
 *        0 - No more columns.
 */
int uiCHIGetNextColumn(UIColumnHeaderIterator* iterator, float scale)
{
    if(!iterator)
        return 0;

    // If there are no more columns, tell the caller there are no more columns.
    if(iterator->columnIndex+1 >= eaSize(&iterator->header->columns))
        return 0;

    // Using the last columns width, figure out where the next column starts.
     iterator->columnStartOffset += iterator->currentWidth;

    // For any column other than the first and last column, add in the separator widths to derive the
    // next column offset.
    if(iterator->columnIndex != -1 && iterator->columnIndex-1 != eaSize(&iterator->header->columns))
        iterator->columnStartOffset += iterator->separatorWidth + (2 * HEADER_SEPARATOR_MARGINS);    

    // Try to get the next column.
    iterator->columnIndex++;
    iterator->column = iterator->header->columns[iterator->columnIndex];
    if (iterator->column->hidden)
        return uiCHIGetNextColumn(iterator, scale);

    if( iterator->column->curWidth < iterator->column->minWidth )
        iterator->column->curWidth = iterator->column->minWidth;
    if( iterator->column->curWidth > iterator->column->maxWidth )
        iterator->column->curWidth = iterator->column->maxWidth;

    if(eaSize(&iterator->header->columns) > 1)
         iterator->currentWidth = iterator->column->curWidth + iterator->extraColumnSpacing;
    else
    {
         unsigned int selectedHeaderPart1Width = 0;
        unsigned int selectedHeaderPart3Width = 0;
           Widget3PartGetWidths(&ColumnSelectedHeaderParts, &selectedHeaderPart1Width, NULL, &selectedHeaderPart3Width);
           iterator->column->curWidth = iterator->currentWidth = iterator->header->width - (selectedHeaderPart1Width + selectedHeaderPart3Width)*scale;
    }
    return 1;
}

 /* Function uiLVHDisplay()
 *    Draws the given header at the specified location on screen.
 *
 *    Draws the header, column names, and selected column indicator.
 */
void uiLVHDisplay(UIListViewHeader* header, float x, float y, float z, float scale)
{
    int penX = x;
    int headerHeight;
    unsigned int selectedHeaderPart1Width = 0;
    unsigned int selectedHeaderPart3Width = 0;
    UIColumnHeaderIterator columnIterator;

    if(!header)
        return;

    uiCHIGetIterator(header, &columnIterator);

    Widget3PartGetWidths(&ColumnSelectedHeaderParts, &selectedHeaderPart1Width, NULL, &selectedHeaderPart3Width);

    // Draw the header base.
    Widget3PartDraw(&ColumnHeaderParts, x, y, z, scale, header->width, header->drawColor);
    headerHeight = Widget3PartHeight(&ColumnHeaderParts)*scale;

    font_color(CLR_WHITE, CLR_WHITE);

    // Draw all headers.
    for(;uiCHIGetNextColumn(&columnIterator, scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        float width;
        CBox box;
        float textCenterX, textCenterY;

        if (column->hidden)
            continue;
           penX = x + columnIterator.columnStartOffset*scale;

        // Figure out how large the column is going to be.
          width = columnIterator.currentWidth*scale;

        // Draw the header text at the center of the column header area.
        box.lx = textCenterX = penX+width/2;
         box.ly = textCenterY = y+headerHeight/2+1*scale;

        // Draw some sprites under the selected column caption.
          if(header->selectedColumn == columnIterator.columnIndex)
        {
            // Find the extents of the string.
            str_dims(font_grp, scale, scale, CENTER_X | CENTER_Y, &box, column->caption);

            // Draw the selected header sprites.
            Widget3PartDraw(&ColumnSelectedHeaderParts, box.upperLeft.x-selectedHeaderPart1Width*scale, y, z+1, scale, 
                 box.hx-box.lx+(selectedHeaderPart1Width+selectedHeaderPart3Width)*scale, header->drawColor);
        }


        if( column->resizable )
        {
            float tx = penX + width + HEADER_SEPARATOR_MARGINS*scale + 1;
            CBox b;
            int color;
            int i;

               BuildCBox( &b, tx-6, y, 13, headerHeight );
            if( column->dragging )
            {
                  color = 0xffffffaa;
                  columnIterator.column->curWidth = MINMAX(gMouseInpCur.x - column->relWD, columnIterator.column->minWidth, columnIterator.column->maxWidth);

                 for( i = 0; i < 4; i++ )
                {
                    if( columnIterator.column->curWidth != columnIterator.column->maxWidth )
                            drawVerticalLine( tx+3+i*1, y + (7+i*2)*scale, headerHeight - (7+i*2)*2*scale, z+5, scale, color );
                    if ( columnIterator.column->curWidth != columnIterator.column->minWidth)
                        drawVerticalLine( tx-3-i*1, y + (7+i*2)*scale, headerHeight - (7+i*2)*2*scale, z+5, scale, color );
                }

                if( !isDown(MS_LEFT) )
                {
                     ShowCursor(1);
                    column->dragging = gListViewDragging = false;
                    saveDividerSettings();
                    sendDividers();
                }

            }
            else if( !gListViewDragging && mouseCollision(&b) )
            {
                color = 0xffffff44;
                 for( i = 0; i < 4; i++ )
                {
                    drawVerticalLine( tx+3+i*1, y + (7+i*2)*scale, headerHeight - (7+i*2)*2*scale, z+5, scale, color );
                    drawVerticalLine( tx-3-i*1, y + (7+i*2)*scale, headerHeight - (7+i*2)*2*scale, z+5, scale, color );
                }
            }
            else
                color = 0x00000088;

              drawVerticalLine( tx, y + 7*scale, headerHeight - 14*scale, z+5, scale, color );

             if( mouseLeftDrag( &b ) && !column->dragging && !gListViewDragging )
            {
                column->dragging = gListViewDragging = true;
                ShowCursor(0);
                column->relWD = gMouseInpCur.x - columnIterator.column->curWidth;
            }

        }

        // Draw the column caption.
         cprntEx(textCenterX, textCenterY, z+2, scale, scale, CENTER_X | CENTER_Y, column->caption);
        penX += width;
    }
}

//--------------------------------------------------------------------------------------------------------------------
// UIListView
//--------------------------------------------------------------------------------------------------------------------
UIListView* uiLVCreate(void)
{
    UIListView* list;
    list = calloc(1, sizeof(UIListView));
    list->header = uiLVHCreate();

    // Initialize the object.
    uiLVSetMinHeight(list, uiLVGetMinDrawHeight(list));
    uiLVSetRowHeight(list, 0);
    list->itemsAreClickable=1;
    return list;
}

void uiLVDestroy(UIListView* list)
{
    if(!list)
        return;
    if(list->header)
        uiLVHDestroy(list->header);
    if(list->items)
        eaDestroy(&list->items);
    if(list->items)
    {
        eaDestroyEx(&list->itemWindows, NULL);
    }
    if(list->compareItem)
    {
        eaDestroyConst(&list->compareItem);
    }
}

void uiLVSetClickable(UIListView* list,int clickable) {
    list->itemsAreClickable=(clickable==0)?0:1;    //clamp it to 0 or 1
}


/* Function uiLVHandleInput()
 *    Handles input for a list view window given its intended draw location.
 *
 *    Deals with header clicks, column sort directions, resorting, and list item mouse over,
 *    and list item clicks.
 *
 *    A column cannot be selected if it cannot be sorted.
 */
void uiLVHandleInput(UIListView* list, PointFloatXYZ origin)
{
    if(!list)
        return;
    
    // Handle list view header clicks.
    // Can the list be sorted at all?
      if(list->compareItem)
    {
        CBox box;
        UIColumnHeaderIterator iterator;
        AtlasTex* arrows[2];
        uiCHIGetIterator(list->header, &iterator);
    
        box.lx = origin.x;
         box.ly = origin.y;
         box.hy = box.ly + uiLVHGetHeight(list->header)*list->scale;

        arrows[0] = atlasLoadTexture("Jelly_arrow_down.tga");
        arrows[1] = atlasLoadTexture("Jelly_arrow_up.tga");

        // Did the user click on one of the headers or the sort arrow?
        // Look through the collision area of all of the column headers.
        for(;uiCHIGetNextColumn(&iterator, list->scale);)
        {
            int arrowClick = 0;

            box.lx = origin.x + iterator.columnStartOffset*list->scale;
            
            // Did the user click on the sort arrow?
            if(list->header->selectedColumn == iterator.columnIndex)
            {
                AtlasTex* arrow = arrows[list->sortDirection];
                CBox sortArrowBox;

                sortArrowBox.lx = box.lx + iterator.currentWidth/2 - arrow->width*list->scale/2;
                sortArrowBox.ly = box.hy;
                sortArrowBox.hy = sortArrowBox.ly + arrow->height*list->scale;
                sortArrowBox.hx = sortArrowBox.lx + arrow->width*list->scale;

                arrowClick = mouseClickHit(&sortArrowBox, MS_LEFT);
            }

            if(iterator.columnIndex == 0)
            {
                // First column.
                 box.hx = box.lx + iterator.currentWidth*list->scale + HEADER_SEPARATOR_MARGINS*list->scale;
            }
            else if(iterator.columnIndex+1 == eaSize(&iterator.header->columns))
            {
                // Last column.
                box.lx -= HEADER_SEPARATOR_MARGINS*list->scale;
                box.hx = box.lx + iterator.currentWidth*list->scale + HEADER_SEPARATOR_MARGINS*list->scale;
            }
            else 
            {
                // Middle columns.
                box.lx -= HEADER_SEPARATOR_MARGINS*list->scale;
                box.hx = box.lx + iterator.currentWidth*list->scale + (2*HEADER_SEPARATOR_MARGINS)*list->scale;
            }

            // Did the mouse click fall within the bounds of this column header?
            if(arrowClick || mouseClickHit(&box, MS_LEFT))
            {
                UIListViewItemCompare itemCompare;

                // Can this column be selected/sorted?
                itemCompare = eaGet(&(void**)list->compareItem, iterator.columnIndex);
                if(!itemCompare)
                    break;
                
                // Reversing sort direction?
                if(list->header->selectedColumn == iterator.columnIndex)
                {
                    list->sortDirection = (list->sortDirection + 1) % UILVSS_MAX;
                }

                // Record which column was selected.
                list->header->selectedColumn = iterator.columnIndex;

                // sort the list item list.
                uiLVSortList(list, list->sortDirection, itemCompare);

                collisions_off_for_rest_of_frame = 1;
                break;
            }
        }
    }

    // Handle list view item clicks.
    if (list->itemsAreClickable)
    {
        int i;
        UIBox listViewItemDrawArea;
        listViewItemDrawArea.x = origin.x;
        listViewItemDrawArea.y = origin.y;
        listViewItemDrawArea.height = list->height;
        listViewItemDrawArea.width = list->header->width;
        uiBoxAlter(&listViewItemDrawArea, UIBAT_SHRINK, UIBAD_TOP, uiLVGetMinDrawHeight(list));
 
        list->mouseOverItem = NULL;
        if(uiMouseCollision(&listViewItemDrawArea))
        {
            for(i = 0; i < eaSize(&list->itemWindows); i++)
            {
                // Did the user click on one of the items?
                  if(uiMouseUpHit(list->itemWindows[i], MS_LEFT) || uiMouseClickHit(list->itemWindows[i], MS_RIGHT))
                {
                    list->selectedItem = list->items[i];
                    list->newlySelectedItem = 1;
                }

                // Did the user mouse over one of the items?
                if(uiMouseCollision(list->itemWindows[i]))
                {
                    list->mouseOverItem = list->items[i];
                }
            }
        }
    }
}


/* Function uiLVDisplayColumnSeparators()
 *    Displays the line in the list view to indicate column boundries.
 *    
 */
static void uiLVDisplayColumnSeparators(UIListView* list, PointFloatXYZ origin)
{
    AtlasTex* separatorMid;
    AtlasTex* separatorEnd;
    UIColumnHeaderIterator columnIterator;
    PointFloatXYZ pen = origin;
    float separatorMidHeight;
    float separatorEndHeight;
    int headerHeight;

    if(!list)
        return;

    separatorMid = atlasLoadTexture("Headerbar_VertDivider_Mid.tga");
    separatorEnd = atlasLoadTexture("Headerbar_VertDivider_Bottom.tga");

    // Draw all seperators right below the header.
    headerHeight = uiLVHGetHeight(list->header);
    pen.y += headerHeight*list->scale;

    // Start iterating through all columns.
    uiCHIGetIterator(list->header, &columnIterator);

    // Skip drawing the separator at the beginning of the first column.
    uiCHIGetNextColumn(&columnIterator, list->scale);

    separatorEndHeight = separatorEnd->height*list->scale;
    separatorMidHeight = list->height - headerHeight - separatorEndHeight + 4*list->scale;
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIBox clipBox;
        pen.x = origin.x + columnIterator.columnStartOffset*list->scale - HEADER_SEPARATOR_MARGINS*list->scale;

        clipBox.origin.x = pen.x;
        clipBox.origin.y = pen.y-4*list->scale;
        clipBox.height = list->height;
         clipBox.width = separatorEnd->width;
        
        //clipperPushRestrict(&clipBox);
        {
             int color = list->drawColor;

            color = (color & 0xffffff00) | 0x00000050;    // Draw everything at 50% opacity

            if(separatorMidHeight)
                display_sprite(separatorMid, pen.x, pen.y-4*list->scale, pen.z, 1.f, separatorMidHeight / separatorMid->height, color);

            display_sprite(separatorEnd, pen.x, pen.y-4*list->scale+separatorMidHeight, pen.z, 1.f, separatorEndHeight / separatorEnd->height, color);
        }
        //clipperPop();
    }
}

/* Function uiLVDisplaySortArrow()
 *    Displays the sort arrows under the list view column headers.
 *    The arrows are not drawn if the selected column cannot be sorted.
 */
int uiLVDisplaySortArrow(UIListView* list, PointFloatXYZ origin)
{
    AtlasTex* arrows[2];
    UIColumnHeaderIterator iterator;
    PointFloatXYZ pen = origin;

    if(!list)
        return 0;

    pen.y += uiLVHGetHeight(list->header)*list->scale;

    // If this list does not know how to compare its items,
    // the list cannot be sorted and does not need sort arrows.
    if(!list->compareItem)
        return 0;
    assert(list->sortDirection >= 0 && list->sortDirection < UILVSS_MAX);

    arrows[0] = atlasLoadTexture("Jelly_arrow_up.tga");
    arrows[1] = atlasLoadTexture("Jelly_arrow_down.tga");

    // If an invalid column is selected or if the column cannot be sorted, don't do anything.
    if(list->header->selectedColumn < 0 || list->header->selectedColumn >= eaSize(&list->header->columns) || !eaGet(&(void**)list->compareItem, list->header->selectedColumn))
    {
        return max(arrows[0]->height, arrows[1]->height);
    }

    uiCHIGetIterator(list->header, &iterator);
    for(;uiCHIGetNextColumn(&iterator, list->scale);)
    {
         if(iterator.columnIndex == list->header->selectedColumn)
        {
            AtlasTex* arrow = arrows[list->sortDirection];
            pen.x = origin.x + iterator.columnStartOffset*list->scale;
             display_sprite(arrow, pen.x + iterator.currentWidth*list->scale/2 - arrow->width*list->scale/2, pen.y -2*list->scale, pen.z, list->scale, list->scale, list->drawColor);
        }
    }
    
    return max(arrows[0]->height, arrows[1]->height);
}

/* Function uiLVGetSortArrowHeight()
 *    Determines the height of the sort arrow.
 */
int uiLVGetSortArrowHeight(UIListView* list)
{
    AtlasTex* arrows[2];
    if(!list || !list->compareItem)
        return 0;
    arrows[0] = atlasLoadTexture("Jelly_arrow_down.tga");
    arrows[1] = atlasLoadTexture("Jelly_arrow_up.tga");
    return max(arrows[0]->height, arrows[1]->height);
}

/* Function uiLVGetSortArrowHeight()
 *    Used to display a UIListView.
 *
 *    Displays the column header, the sort arrows, and calls the display item callback to
 *    display all items.
 */
void uiLVDisplay(UIListView* list, PointFloatXYZ origin)
{
    int i;
    PointFloatXYZ pen = origin;
    UIBox clipBox;
    unsigned int minDrawHeight;

    if(!list || !list->displayItem)
        return;
    
    // Draw the list view header.
     uiLVHDisplay(list->header, pen.x, pen.y, pen.z+1, list->scale);
    uiLVDisplayColumnSeparators(list, pen);    
    uiLVDisplaySortArrow(list, pen);
    minDrawHeight = uiLVGetMinDrawHeight(list);

    // Adjust the pen so that it draws right below the sort arrows or column headers.
    pen.y = pen.y + minDrawHeight*list->scale;
    pen.z += 1;                        // All items draw above the list view controls.

    // Clip out everything we don't want.
     clipBox.x = pen.x;
     clipBox.y = pen.y - PIX2;
     clipBox.width = list->header->width;
     clipBox.height = list->height - minDrawHeight;
    clipperPushRestrict(&clipBox);
    

    // Adjust the pen so that items are scrolled appropriately.
     pen.y -= list->scrollOffset;

    for(i = 0; i < eaSizeUnsafe(&list->items); i++)
    { 
        void* item = list->items[i];
        UIBox* box = eaGet(&list->itemWindows, i);

        assert(box);    // Always managed along with item addition + removal.  Should always exist.

        *box = list->displayItem(list, pen, list->userSettings, item, i);

        // Draw selection background first if the item is selected.
        // FIXME!!!  Get rid of these +PIX3 things by first figuring out the area in which the control should be allowed to be drawn.
        if(item == list->selectedItem)
        {            
             int foregrounColor = CLR_SELECTION_FOREGROUND;
             int backgrounColor = CLR_SELECTION_BACKGROUND;
             drawFlatFrame(PIX2, R10, pen.x+PIX3*list->scale, pen.y, origin.z, list->header->width-2*PIX3*list->scale, (R10+PIX2)*2*list->scale, list->scale, foregrounColor, backgrounColor);
        }
        if(item == list->mouseOverItem)
        {
            int foregrounColor = CLR_MOUSEOVER_FOREGROUND;
            int backgrounColor = CLR_MOUSEOVER_BACKGROUND;
            drawFlatFrame(PIX2, R10, pen.x+PIX3*list->scale, pen.y, origin.z, list->header->width-2*PIX3*list->scale, (R10+PIX2)*2*list->scale, list->scale, foregrounColor, backgrounColor);
        }

        // Assuming items are all uniform in height.
        // Note that row height is limited to the selection highlight height.

        // Alternatively, let each item decide how tall it is supposed to be by commenting this line out.
        // This requires that all display item function to space themselves out correctly.
        box->x = pen.x;
         box->y = pen.y;
        box->width = list->header->width;
        box->height = list->rowHeight*list->scale;
        pen.y += box->height;
    }
    clipperPop();
}

int uiLVSetWidth(UIListView* list, unsigned int width)
{
    if(!list)
        return 0;
    return uiLVHSetMinWidth(list->header, width);
}

int uiLVSetHeight(UIListView* list, unsigned int height)
{
    if(!list || height < list->minHeight)
        return 0;
    
    list->height = height;
    return 1;
}

unsigned int uiLVGetHeight(UIListView* list)
{
    if(!list)
        return 0;
    return list->height;
}

void uiLVSetScale(UIListView* list, float scale)
{
    list->scale = scale;
}

int uiLVSetMinHeight(UIListView* list, unsigned int height)
{
    if(!list || height < uiLVGetMinDrawHeight(list))
        return 0;

    list->minHeight = height;
    return 1;
}

unsigned int uiLVGetMinDrawHeight(UIListView* list)
{
    int minDrawHeight = 0;

    if(!list)
        return 0;

    // Find the list header's draw height.
    minDrawHeight += uiLVHGetHeight(list->header);

    // Find the list sort arrow draw height.
    minDrawHeight += uiLVGetSortArrowHeight(list);

    return minDrawHeight;
}

unsigned int uiLVGetFullDrawHeight(UIListView* list)
{
    UIBox* lastItemWindow;

    if(!list)
        return 0;

    // If there are no items in the list view, return the minimum draw height.
    if(!list->itemWindows || 0 == eaSize(&list->itemWindows))
        return uiLVGetMinDrawHeight(list);

    lastItemWindow = eaGet(&list->itemWindows, eaSize(&list->itemWindows)-1);
    if(!lastItemWindow)
        return uiLVGetMinDrawHeight(list);

    // lastItemWindow->y + lastItemWindow->height + list->scrollOffset;
    return list->rowHeight * eaSizeUnsafe(&list->items) + uiLVGetMinDrawHeight(list);
}

unsigned int uiLVSetRowHeight(UIListView* list, unsigned int height)
{
    if(!list)
        return 0;
    
    if(LIST_SELECTION_MIN_HEIGHT > height)
        height = LIST_SELECTION_MIN_HEIGHT;

    list->rowHeight = height;
    return height;
}

int uiLVFinalizeSettings(UIListView* list)
{
    if(!list)
        return 0;

    uiLVHFinalizeSettings(list->header);

    return 1;
}

void uiLVSetDrawColor(UIListView* list, int color)
{
    if(!list)
        return;
    list->drawColor = color;
    uiLVHSetDrawColor(list->header, color);
}

void uiLVEnableSelection(UIListView* list, int enable)
{
    if(!list)
        return;
    list->enableSelection = enable;
    if(!list->enableSelection)
        list->selectedItem = NULL;
}

void uiLVEnableMouseOver(UIListView* list, int enable)
{
    if(!list)
        return;
    list->enableMouseOver = enable;
    if(!list->enableMouseOver)
        list->mouseOverItem = NULL;
}

int uiLVAddItem(UIListView* list, void* item)
{
    int itemIndex, itemWindowIndex;
    if(!list)
        return -1;

    itemIndex = eaPush(&list->items, item);
    itemWindowIndex = eaPush(&list->itemWindows, uiBoxCreate());
    assert(itemIndex == itemWindowIndex);
    return itemIndex;
}

int uiLVFindItem(UIListView* list, void* item)
{
    if(!list)
        return -1;
    return eaFind(&list->items, item);
}

int uiLVRemoveItem(UIListView* list, void* item)
{
    int index;
    UIBox* box;

    if(!list)
        return 0;

    index = uiLVFindItem(list, item);
    if(index < 0)
        return 0;
    eaRemove(&list->items, index);

    box = eaRemove(&list->itemWindows, index);
    assert(box);
    uiBoxDestroy(box);

    if(list->selectedItem == item)
        list->selectedItem = NULL;

    return 1;
}

int uiLVSelectItem(UIListView* list, unsigned int itemIndex)
{
    if(!list)
        return 0;

    list->selectedItem = eaGet(&list->items, itemIndex);
    if(!list->selectedItem)
        return 0;

    return 1;
}

int uiLVClear( UIListView* list )
{
    return uiLVClearEx( list, NULL );
}

static void uiBoxDestroyAdapter(void* arg0)
{
    uiBoxDestroy((UIBox*)arg0);
}

int uiLVClearEx(UIListView* list, void (*destructor)(void*) )
{
    if(!list)
        return 0;

    if( destructor )
        eaClearEx(&list->items, destructor);
    else
        eaSetSize(&list->items, 0);
    eaClearEx(&list->itemWindows, uiBoxDestroyAdapter);
    list->selectedItem = NULL;
    return 1;
}

typedef struct 
{
    UIListViewItemCompare itemCompare;
    UIListViewSortDirection sortDirection;
} UIListViewSortContext;

static int uiListViewSortCompare(const void* item1, const void* item2, const void* contextIn)
{
    const UIListViewSortContext* context = contextIn;
    int result = context->itemCompare(item1, item2);

    if(context->sortDirection == UILVSS_BACKWARD)
        result *= -1;

    return result;
}

int uiLVSortList(UIListView* list, UIListViewSortDirection sortDirection, UIListViewItemCompare itemCompare)
{
    UIListViewSortContext context = {0};

    if(!list || !itemCompare)
        return 0;

    context.sortDirection = sortDirection;
    context.itemCompare = itemCompare;
    
    contextQsort(list->items, eaSizeUnsafe(&list->items), sizeof(void*), &context, uiListViewSortCompare);
    return 1;
}

int uiLVSortBySelectedColumn(UIListView* list)
{
    UIListViewItemCompare itemCompare;
    if(!list)
        return 0;

    itemCompare = eaGet(&(void**)list->compareItem, list->header->selectedColumn);
    if(!itemCompare)
        return 0;

    uiLVSortList(list, list->sortDirection, itemCompare);
    return 1;
}

/* Function uiLVBindCompareFunction()
 *    This functions binds the item compare function for the specified column.
 */
int uiLVBindCompareFunction(UIListView* list, char* columnName, UIListViewItemCompare itemCompare)
{
    int columnIndex;
    if(!list || !columnName || !itemCompare)
        return 0;

    columnIndex = uiLVHFindColumnIndex(list->header, columnName);
    if(columnIndex < 0)
        return 0;

    if(!list->compareItem)
    {
        eaCreate(&(void**)list->compareItem);
        list->header->selectedColumn = columnIndex;
    }
    eaSetSize(&(void**)list->compareItem, columnIndex+1);
    eaSet(&(void**)list->compareItem, itemCompare, columnIndex);
    return 1;
}


void setSavedDividerSettings(void)
{
    int i;
    for( i = 0; i < ARRAY_SIZE(savedColumns); i++ )
    {
        if( savedColumns[i].column )
            savedColumns[i].column->curWidth = *(savedColumns[i].width);
    }
}

void saveDividerSettings(void)
{
    int i;
    for( i = 0; i < ARRAY_SIZE(savedColumns); i++ )
    {
        if( savedColumns[i].column )
            *(savedColumns[i].width) = savedColumns[i].column->curWidth;
    }

}
