# PolygonFromSprite

![Example](https://github.com/mjjq/PolygonFromSprite/blob/master/images/example.jpg)

## What does this library do?

This is a header only C++ library which creates polygons (arrays of vertices) from input PNG sprites.

## Requirements

SFML-2.4.2 or later is required as this library makes use of SFML's sf::Image class for loading, reading, and modifying input images.

## Installation

Simply copy ``` VerticesFromBitmap.hpp ``` to your project and ``` #include ``` the header.

## Usage

Create a ``` VerticesFromBitmap ``` object and call ``` VerticesFromBitmap::generateIslands(image) ``` where ``` image ``` is of type ```sf::Image ``` . 
The function will return an array of ```Island ```; each ``` Island ``` is an array of ``` Vec2 ``` positions representing vertices of a closed polygon.

## Demo

This repo comes with a demo to try out before usage:

1) Place any ``` .png ``` file named ``` test.png ``` into the same directory as the ``` .exe ```. 
2) Open the ``` .exe ``` file and the polygon will automatically be rendered.

### Controls
Reload image: ``` R ```

Display ``` .png ``` image in background: ``` D ```

Switch between aliased and original image: ``` S ```

Double minimum distance between vertices: ``` Num1 ``` 

Halve minimum distance between vertices: ``` Num2 ```

Double vertex rendered size: ``` PgUp ```

Halve vertex rendered size: ``` PgDn ```

Increase co-linear threshold: ``` L ```

Decrease co-linear threshold: ``` K ```







