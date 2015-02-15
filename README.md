ofxCentralizedGUI
=====================================

Introduction
------------
This addon is designed to be used for user interfaces that are moderately complex. If you have a need for a fairly complex GUI, you might want to look at something like Qt or another real GUI software package. If you need a really simple GUI, this might be overkill.

Given where this addon is in development (almost no documentation and limited features), I can't recommend that you use this. However, I think there are some good ideas in how I structured things, the advantages of which I describe below.

The "centralized" in the name refers to the fact that all of the UI is managed by a single class, as opposed to how many UIs treat each control independently. The problem with treating each UI control (most people call them widgets, I call the controls) independently is that controls can interact with one another in annoying ways. For example, if a dropdown menu covers a button and the selection in the dropdown that is clicked on is at the same location as the button, if each control is independent they can't tell which was supposed to receive the click (or even that there was another control that could have received the click).

This GUI attempts to scale well by providing a single point from which user code can interface with all controls. To find out what is happening with the UI, you only need to call the getNextInteraction() function to loop through all of the UI interactions and process them in one place. To me, dealing with all UI interactions in one place helps keep me sane when dealing with a complex UI. Additionally, all controls are drawn with a single function call of the managing class, which allows for more control over when controls are drawn. You can make different controls visible/invisible, enabled/disabled, etc. You can set the Z-Order of controls to manage which controls are on top (controls that are on top are the only controls that get mouse events). Another advantage is that because all of the controls are given each user interaction at once by the manaing class, the managing class can know when none of the controls received a given keypress and as a result it can treat it as a keyboard shortcut. This is impossible with independent controls.

This addon is designed to be efficient in resource use. For example, drawing a large amount of text can be slow using ofTrueTypeFont::drawString(), because a lot of computation must be performed for each string. For that reason, ofxCentrallizedGui uses drawStringAsShapes() to draw text. However, if each control has its own instance of an ofTrueTypeFont for which the font contours have been generated (which you must do in order to use drawStringAsShapes()), the program begins to take up a very large amount of memory. To solve this problem, a single ofTrueTypeFont instance is shared by all of the controls by default, although if you want to give specific controls different fonts, you can do so. Another way in which it tries to be efficient is that you can check if any of the controls have been interacted with in such a way that they need to be redrawn. If nothing has changed, you don't have to redraw the entire UI. If you have other stuff other than the UI that is changing, you could draw the UI into an FBO with a transparent background and just draw the FBO each frame, only redrawing the UI into the FBO when the UI changes.

One of the core concepts of this addon is how to identify controls, because controls are stored within a manager class and can only be accessed with identifying strings. There are two pieces of information that can be used to identify a control: it's name and it's groups. A control name should be unique to each control, at least within each combination of control groups. For example, you might have a dropdown with the name "ageDropdown" within the "demographics" control group, but you can also have an "ageDropdown" within the "fileInfo" control group. In this example, you will be able to uniquely identify each control with a combination of name and group information.

Controls can have multiple groups. For example, imagine that you have text boxes for phone numbers and you want to collect cell phone and work phone numbers, with separate area code for each. You might have a text box with the groups "contactInformation,workPhone" and the name "areaCode", where where "," delimits groups from one another. You can then add another text box with the name "localNumber" and the same groups to allow for unique identification.

It is possible to apply some functions to control groups. For example, you might want to highlight some subset of your controls by changing the background color of the control. You can do this with `UI_manager::setGroupComponentColor()`. In an example call, you might say
~~~
UI.setGroupComponentColor("workPhone", "background", ofColor::red);
~~~
This subsets the controls and only selects those that have the group "workPhone" and sets the "background" color to those controls.

Licence
-------
This code is distributed under the MIT license (see license.md).

Installation
------------
Just download the repo and drop it into the addons directory.

Dependencies
------------
None.

Compatibility
------------
This has only really been tested with oF versions 0.8.0 and 0.8.4, but there is no clear reason why it would not work with other versions.

Known issues
------------
Limited number of UI controls. Some of the controls are not very fully featured (like the TextBox).

Version history
------------

0.1.0 -- Initial release.

