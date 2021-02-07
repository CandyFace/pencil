/*

Pencil2D - Traditional Animation Software
Copyright (C) 2012-2020 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/
#include "catch.hpp"

#include "layer.h"
#include "layerbitmap.h"
#include "layervector.h"
#include "layercamera.h"
#include "layersound.h"
#include "object.h"
#include "util.h"


TEST_CASE("LayerType")
{
    Object* object = new Object;

    SECTION("Bitmap Layer")
    {
        Layer* bitmapLayer = new LayerBitmap(object);
        REQUIRE(bitmapLayer->type() == Layer::BITMAP);
        delete bitmapLayer;
    }
    SECTION("Vector Layer")
    {
        Layer* vecLayer = new LayerVector(object);
        REQUIRE(vecLayer->type() == Layer::VECTOR);
        delete vecLayer;
    }
    SECTION("Camera Layer")
    {
        Layer* cameraLayer = new LayerCamera(object);
        REQUIRE(cameraLayer->type() == Layer::CAMERA);
        delete cameraLayer;
    }
    SECTION("Sound Layer")
    {
        Layer* soundLayer = new LayerSound(object);
        REQUIRE(soundLayer->type() == Layer::SOUND);
        delete soundLayer;
    }

    delete object;
}

SCENARIO("Add key frames into a Layer", "[Layer]")
{
    Object* object = new Object;

    GIVEN("A Bitmap Layer")
    {
        Layer* layer = new LayerBitmap(object);

        REQUIRE(layer->addNewKeyFrameAt(0) == false); // first key position is 1.
        REQUIRE(layer->keyFrameCount() == 0);

        WHEN("Add a keyframe")
        {
            REQUIRE(layer->addNewKeyFrameAt(1) == true);

            THEN("keyframe can be found in the layer")
            {
                REQUIRE(layer->getMaxKeyFramePosition() == 1);
                REQUIRE(layer->keyFrameCount() == 1);
                REQUIRE(layer->getKeyFrameAt(1) != nullptr);
            }
        }

        WHEN("Add 2 keyframes")
        {
            REQUIRE(layer->addNewKeyFrameAt(1) == true);
            REQUIRE(layer->addNewKeyFrameAt(3) == true);

            THEN("2 keyframes are in the layer")
            {
                REQUIRE(layer->getMaxKeyFramePosition() == 3);
                REQUIRE(layer->getKeyFrameAt(3) != nullptr);
                REQUIRE(layer->keyFrameCount() == 2);
            }
        }
        delete layer;
    }

    GIVEN("A Vector Layer")
    {
        Layer* layer = new LayerVector(object);

        REQUIRE(layer->addNewKeyFrameAt(0) == false); // first key position is 1.
        REQUIRE(layer->keyFrameCount() == 0);

        WHEN("Add a keyframe")
        {
            REQUIRE(layer->addNewKeyFrameAt(1) == true);

            THEN("keyframe can be found in the layer")
            {
                REQUIRE(layer->getMaxKeyFramePosition() == 1);
                REQUIRE(layer->keyFrameCount() == 1);
                REQUIRE(layer->getKeyFrameAt(1) != nullptr);
            }
        }
        delete layer;
    }

    GIVEN("A Camera Layer")
    {
        Layer* layer = new LayerCamera(object);

        REQUIRE(layer->addNewKeyFrameAt(0) == false); // first key position is 1.
        REQUIRE(layer->keyFrameCount() == 0);

        WHEN("Add a keyframe")
        {
            REQUIRE(layer->addNewKeyFrameAt(1) == true);

            THEN("keyframe can be found in the layer")
            {
                REQUIRE(layer->getMaxKeyFramePosition() == 1);
                REQUIRE(layer->keyFrameCount() == 1);
                REQUIRE(layer->getKeyFrameAt(1) != nullptr);
            }
        }
        delete layer;
    }
    delete object;
}

TEST_CASE("Test Layer::keyExists()", "[Layer]")
{
    Object* obj = new Object;

    SECTION("Fresh new Layer")
    {
        Layer* layer = obj->addNewBitmapLayer();
        REQUIRE(layer->keyExists(1) == true); // there is a frame at 1 in default.

    }
    SECTION("Key exists at 15")
    {
        Layer* layer = obj->addNewBitmapLayer();
        REQUIRE(layer->addNewKeyFrameAt(15));
        REQUIRE(layer->keyExists(16) == false);
        REQUIRE(layer->keyExists(15) == true);
        REQUIRE(layer->keyExists(14) == false);
        REQUIRE(layer->keyExists(13) == false);
        REQUIRE(layer->keyExists(12) == false);
    }
    SECTION("Key exists 10 20 30")
    {
        Layer* layer = obj->addNewBitmapLayer();

        REQUIRE(layer->addNewKeyFrameAt(10));
        REQUIRE(layer->keyExists(10) == true);

        REQUIRE(layer->addNewKeyFrameAt(20));
        REQUIRE(layer->keyExists(20) == true);

        REQUIRE(layer->addNewKeyFrameAt(30));
        REQUIRE(layer->keyExists(30) == true);

        // test false case
        REQUIRE(layer->keyExists(0) == false);
        REQUIRE(layer->keyExists(21) == false);
        REQUIRE(layer->keyExists(31) == false);
        REQUIRE(layer->keyExists(1000) == false);
        REQUIRE(layer->keyExists(-333) == false);
    }

    delete obj;
}

TEST_CASE("Layer::firstKeyFramePosition()")
{
    Object* obj = new Object;

    SECTION("At least one key")
    {
        Layer* layer = obj->addNewBitmapLayer();
        REQUIRE(layer->firstKeyFramePosition() == 1);

        layer->addNewKeyFrameAt(99);
        REQUIRE(layer->firstKeyFramePosition() != 0);

        layer->moveKeyFrame(1, 1);
        REQUIRE(layer->firstKeyFramePosition() != 0); // one keyframe has to exist
    }

    delete obj;
}

TEST_CASE("Layer::getMaxKeyFramePosition()")
{
    Object* obj = new Object;

    SECTION("Bitmap")
    {
        Layer* layer = obj->addNewBitmapLayer();

        // 1 at beginning.
        REQUIRE(layer->getMaxKeyFramePosition() == 1);

        CHECK(layer->addNewKeyFrameAt(3));
        REQUIRE(layer->getMaxKeyFramePosition() == 3);

        CHECK(layer->addNewKeyFrameAt(8));
        REQUIRE(layer->getMaxKeyFramePosition() == 8);

        CHECK(layer->addNewKeyFrameAt(100));
        REQUIRE(layer->getMaxKeyFramePosition() == 100);

        CHECK(layer->addNewKeyFrameAt(80));
        REQUIRE(layer->getMaxKeyFramePosition() == 100);
    }

    delete obj;
}


TEST_CASE("Layer::removeKeyFrame()")
{
    Object* obj = new Object;
    SECTION("Bitmap")
    {
        Layer* layer = obj->addNewBitmapLayer();

        // there is always a key at position 1 at beginning
        CHECK(layer->removeKeyFrame(1));
        REQUIRE(layer->getMaxKeyFramePosition() == 0);

        for (int i = 2; i <= 20; ++i)
        {
            CHECK(layer->addNewKeyFrameAt(i));
        }

        REQUIRE(layer->keyExists(20) == true);
        layer->removeKeyFrame(20); // after deleting it, the key will no longer exist.
        REQUIRE(layer->keyExists(20) == false);

        REQUIRE(layer->keyExists(8) == true);
        layer->removeKeyFrame(8);
        REQUIRE(layer->keyExists(8) == false);

        REQUIRE(layer->keyExists(19) == true);
        layer->removeKeyFrame(19);
        REQUIRE(layer->keyExists(19) == false);
        REQUIRE(layer->getMaxKeyFramePosition() == 18); // also decrease the max position if we delete the last key.

        layer->removeKeyFrame(18);
        REQUIRE(layer->getMaxKeyFramePosition() == 17);
    }
    delete obj;
}

TEST_CASE("Layer::getPreviousKeyFramePosition()")
{
    Object* obj = new Object;
    SECTION("KeyFrame 1")
    {
        Layer* layer = obj->addNewBitmapLayer();
        CHECK(layer->keyFrameCount() == 1);

        REQUIRE(layer->getPreviousKeyFramePosition(1) == 1);
        REQUIRE(layer->getPreviousKeyFramePosition(10) == 1);
        REQUIRE(layer->getPreviousKeyFramePosition(100) == 1);
        REQUIRE(layer->getPreviousKeyFramePosition(1000) == 1);
    }

    SECTION("KeyFrame 1, 2, 8")
    {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(2);
        layer->addNewKeyFrameAt(8);
        REQUIRE(layer->getPreviousKeyFramePosition(2) == 1);
        REQUIRE(layer->getPreviousKeyFramePosition(8) == 2);
    }

    SECTION("KeyFrame 1, 15")
    {
        Layer* layer = obj->addNewBitmapLayer();
        REQUIRE(layer->getPreviousKeyFramePosition(-5) == 1);

        layer->addNewKeyFrameAt(15);
        REQUIRE(layer->getPreviousKeyFramePosition(16) == 15);
        REQUIRE(layer->getPreviousKeyFramePosition(17) == 15);
    }

    SECTION("KeyFrame 1, 10, Add 20 Remove 20")
    {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(10);
        layer->addNewKeyFrameAt(20);

        REQUIRE(layer->getPreviousKeyFramePosition(21) == 20);
        layer->removeKeyFrame(20);
        REQUIRE(layer->getPreviousKeyFramePosition(21) == 10);
    }
    delete obj;
}

TEST_CASE("Layer::getNextKeyFramePosition()")
{
    Object* obj = new Object;
    SECTION("KeyFrame 1, 5")
    {
        Layer* layer = obj->addNewBitmapLayer();

        REQUIRE(layer->getNextKeyFramePosition(1) == 1);
        REQUIRE(layer->getNextKeyFramePosition(10) == 1);
        REQUIRE(layer->getNextKeyFramePosition(100) == 1);

        CHECK(layer->addNewKeyFrameAt(5));
        REQUIRE(layer->getNextKeyFramePosition(1) == 5);
        REQUIRE(layer->getNextKeyFramePosition(2) == 5);
    }
    delete obj;
}

TEST_CASE("Layer::getPreviousFrameNumber()")
{
    Object* obj = new Object;
    SECTION("KeyFrame 1")
    {
        Layer* layer = obj->addNewVectorLayer();
        REQUIRE(layer->getPreviousFrameNumber(1, true) == -1); // couldn't find previous frame

        REQUIRE(layer->getPreviousFrameNumber(3, true) == 1);
        REQUIRE(layer->getPreviousFrameNumber(3, false) == 2);
    }
    delete obj;
}

TEST_CASE("Layer::increaseFrameExposure")
{
    Object* obj = new Object;
    SECTION("Increase exposure linear")
    {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(1);
        layer->addNewKeyFrameAt(2);
        layer->addNewKeyFrameAt(3);

        layer->addNewKeyFrameAt(5);
        layer->addNewKeyFrameAt(6);

        layer->setFrameSelected(1, true);
        layer->setFrameSelected(2, true);
        layer->setFrameSelected(3, true);

        layer->addExposureToSelectedFrames(1);

        REQUIRE(layer->selectedKeyFrameCount() == 3);
        REQUIRE(layer->isFrameSelected(1));
        REQUIRE(layer->isFrameSelected(3));
        REQUIRE(layer->isFrameSelected(5));

        // Check that the frames that was in front of the last element in the selection
        // has also been moved
        REQUIRE(layer->keyExists(8));
        REQUIRE(layer->keyExists(9));
    }

    SECTION("Increase exposure irregular spacing")
    {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(15);
        layer->addNewKeyFrameAt(18);
        layer->addNewKeyFrameAt(20);

        layer->addNewKeyFrameAt(21);
        layer->addNewKeyFrameAt(24);

        layer->setFrameSelected(24, true);
        layer->setFrameSelected(21, true);
        layer->setFrameSelected(18, true);
        layer->setFrameSelected(15, true);
        layer->setFrameSelected(20, true);

        layer->addExposureToSelectedFrames(2);

        REQUIRE(layer->selectedKeyFrameCount() == 5);
        REQUIRE(layer->isFrameSelected(15));
        REQUIRE(layer->isFrameSelected(20));
        REQUIRE(layer->isFrameSelected(24));
        REQUIRE(layer->isFrameSelected(27));
        REQUIRE(layer->isFrameSelected(32));

        REQUIRE(layer->selectedKeyFramesByLast().at(0) == 24);
        REQUIRE(layer->selectedKeyFramesByLast().at(1) == 15);
        REQUIRE(layer->selectedKeyFramesByLast().at(2) == 20);
        REQUIRE(layer->selectedKeyFramesByLast().at(3) == 27);
        REQUIRE(layer->selectedKeyFramesByLast().at(4) == 32);
    }

    SECTION("Increase exposure frames in between")
    {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(19);
        layer->addNewKeyFrameAt(20);
        layer->addNewKeyFrameAt(21);
        layer->addNewKeyFrameAt(22);
        layer->addNewKeyFrameAt(23);

        layer->setFrameSelected(19, true);
        layer->setFrameSelected(21, true);
        layer->setFrameSelected(23, true);

        layer->addExposureToSelectedFrames(2);

        REQUIRE(layer->selectedKeyFrameCount() == 3);
        REQUIRE(layer->isFrameSelected(19));
        REQUIRE(layer->isFrameSelected(23));
        REQUIRE(layer->isFrameSelected(27));

        REQUIRE(layer->selectedKeyFramesByLast().at(0) == 27);
        REQUIRE(layer->selectedKeyFramesByLast().at(1) == 23);
        REQUIRE(layer->selectedKeyFramesByLast().at(2) == 19);
    }
    delete obj;
}

TEST_CASE("layer::reverseOrderOfSelection()") {
    Object* obj = new Object;

    SECTION("No frames selected") {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(6);
        layer->addNewKeyFrameAt(9);
        layer->addNewKeyFrameAt(13);
        layer->addNewKeyFrameAt(15);
        layer->addNewKeyFrameAt(18);

        REQUIRE(layer->reverseOrderOfSelection() == false);
    }

    SECTION("Reverse selection") {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(6);
        layer->addNewKeyFrameAt(9);
        layer->addNewKeyFrameAt(13);
        layer->addNewKeyFrameAt(15);
        layer->addNewKeyFrameAt(18);

        auto frame1 = layer->getKeyFrameAt(6);
        auto frame2 = layer->getKeyFrameAt(9);
        auto frame3 = layer->getKeyFrameAt(13);
        auto frame4 = layer->getKeyFrameAt(15);
        auto frame5 = layer->getKeyFrameAt(18);

        layer->setFrameSelected(6, true);
        layer->setFrameSelected(9, true);
        layer->setFrameSelected(13, true);
        layer->setFrameSelected(15, true);
        layer->setFrameSelected(18, true);

        REQUIRE(layer->reverseOrderOfSelection());

        // Check that the content has been swapped properly
        REQUIRE(layer->getKeyFrameAt(18) == frame1);
        REQUIRE(layer->getKeyFrameAt(15) == frame2);
        REQUIRE(layer->getKeyFrameAt(13) == frame3);
        REQUIRE(layer->getKeyFrameAt(9) == frame4);
        REQUIRE(layer->getKeyFrameAt(6) == frame5);

        REQUIRE(layer->getKeyFrameAt(6) != frame1);
        REQUIRE(layer->getKeyFrameAt(9) != frame2);
        REQUIRE(layer->getKeyFrameAt(13) == frame3);
        REQUIRE(layer->getKeyFrameAt(15) != frame4);
        REQUIRE(layer->getKeyFrameAt(18) != frame5);
    }
    delete obj;
}

TEST_CASE("layer::insertExposureAt(int position)") {
    Object* obj = new Object;

    SECTION("Add exposure to frame 3") {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(3);
        layer->addNewKeyFrameAt(4);
        layer->addNewKeyFrameAt(5);

        layer->insertExposureAt(3);

        // Check that frames has been moved
        REQUIRE(layer->keyExists(3));
        REQUIRE(layer->keyExists(5));
        REQUIRE(layer->keyExists(6));

        // Check that newly exposed frame position doesn't contain a frame
        REQUIRE(layer->keyExists(4) == false);
    }

    SECTION("invalid frame") {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(3);
        layer->addNewKeyFrameAt(4);
        layer->addNewKeyFrameAt(5);

        REQUIRE(layer->insertExposureAt(-1) == false);
    }


    SECTION("Insert next to frame") {
        Layer* layer = obj->addNewBitmapLayer();
        layer->addNewKeyFrameAt(3);
        layer->addNewKeyFrameAt(4);

        REQUIRE(layer->insertExposureAt(2) == false);
        REQUIRE(layer->keyExists(3));
        REQUIRE(layer->keyExists(4));
    }
    delete obj;
}


//TEST_CASE("Layer::")
