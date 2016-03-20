package navigation;

/*
 * @author daniel beard
 * http://danielbeard.wordpress.com
 * http://github.com/paintstripper
 *
 * Copyright (C) 2012 Daniel Beard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

@SuppressWarnings("rawtypes")
public class State implements Comparable, java.io.Serializable {
	private static final long serialVersionUID = 4708066414355847510L;
	public int x=0;
	public int y=0;
	public Pair<Double, Double> k = new Pair<Double, Double>(0.0,0.0);


	//Default constructor
	public State() {

	}

	//Overloaded constructor
	public State(int x, int y, Pair<Double,Double> k) {
		this.x = x;
		this.y = y;
		this.k = k;
	}

	//Overloaded constructor
	public State(State other) {
		this.x = other.x;
		this.y = other.y;
		this.k = other.k;
	}

	//Equals
	public boolean eq(final State s2) {
		return ((this.x == s2.x) && (this.y == s2.y));
	}

	//Not Equals
	public boolean neq(final State s2) {
		return ((this.x != s2.x) || (this.y != s2.y));
	}

	//Greater than
	public boolean gt(final State s2) {
		if (k.first()-0.00001 > s2.k.first()) return true;
		else if (k.first() < s2.k.first()-0.00001) return false;
		return k.second() > s2.k.second();
	}

	//Less than or equal to
	public boolean lte(final State s2) {
		if (k.first() < s2.k.first()) return true;
		else if (k.first() > s2.k.first()) return false;
		return k.second() < s2.k.second() + 0.00001;
	}

	//Less than
	public boolean lt(final State s2) {
		if (k.first() + 0.000001 < s2.k.first()) return true;
		else if (k.first() - 0.000001 > s2.k.first()) return false;
		return k.second() < s2.k.second();
	}

	//CompareTo Method. This is necessary when this class is used in a priority queue
	public int compareTo(Object that) {
		//This is a modified version of the gt method
		State other = (State)that;
		if (k.first()-0.00001 > other.k.first()) return 1;
		else if (k.first() < other.k.first()-0.00001) return -1;
		if (k.second() > other.k.second()) return 1;
		else if (k.second() < other.k.second()) return -1;
		return 0;
	}

	//Override the CompareTo function for the HashMap usage
	@Override
	public int hashCode() {
		return this.x + 34245*this.y;
	}

	@Override public boolean equals(Object aThat) {
		//check for self-comparison
		if ( this == aThat ) return true;

		//use instanceof instead of getClass here for two reasons
		//1. if need be, it can match any supertype, and not just one class;
		//2. it renders an explict check for "that == null" redundant, since
		//it does the check for null already - "null instanceof [type]" always
		//returns false. (See Effective Java by Joshua Bloch.)
		if ( !(aThat instanceof State) ) return false;
		//Alternative to the above line :
		//if ( aThat == null || aThat.getClass() != this.getClass() ) return false;

		//cast to native object is now safe
		State that = (State)aThat;

		//now a proper field-by-field evaluation can be made
		if (this.x == that.x && this.y == that.y) return true;
		return false;

	}

}
