package Glyph;

import Window.Window;

import java.util.ArrayList;
import java.util.List;

/**
 * Defines a composite glyph which is a glyph that has children
 */
public abstract class CompositeGlyph extends Glyph {
	protected List<Glyph> childList;

	public CompositeGlyph(){
		childList = new ArrayList<Glyph>();
	}

    @Override
    public void insert(Window window, Glyph glyph){
        glyph.setParent(this);
        childList.add(glyph);
    }
	@Override
	public Glyph find(int x, int y){
		Point point = new Point();
		point.setCoords(x, y);
		Glyph commandGlyph = null;

		if(getBounds().intersects(point)){
			if(this.getCommand() != null){
				commandGlyph = this;
			}
			for(Glyph childGlyph: childList){
				Glyph foundGlyph = childGlyph.find(x, y);
				if(foundGlyph != null){
					if(foundGlyph.getCommand() != null){
						commandGlyph = foundGlyph;
					}
				}
			}
		}
		return commandGlyph;
	}

	@Override
	public void insert(Window window, Glyph glyph, int position){

		// We will set the parent equal to this glyph
		glyph.setParent(this);
		if(position < childList.size()){
			// Create a new list to add to
			List<Glyph> newList = new ArrayList<Glyph>();

			// Get list items before the given position
			List<Glyph> beforeList = childList.subList(0, position);

			// Get list items after the given position
			List<Glyph> afterList = childList.subList(position, childList.size());

			// Add items before position
			newList.addAll(beforeList);

			// Add glyph at specified position by user
			newList.add(glyph);

			// Add the rest of the items to the end of the list.
			newList.addAll(afterList);
			childList = newList;

		}
		else {
			// If a user gives an index out of bounds or equal to the current length
			// Append to end
			childList.add(glyph);
		}
	}

	@Override
	public void remove(Window window, Glyph glyph){
		// Removes the specified glyph.
		childList.remove(glyph);
	}

	@Override
	public void draw(Window window){
		// Composite Glyphs have children, so they should draw each of its children
		for(Glyph child: childList){
			child.draw(window);
		}
	}

	@Override
	public Glyph child(int position){
		return childList.get(position);
	}
}
