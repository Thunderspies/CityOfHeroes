#############################################################
## Wolf Spider Helmet - Glow
#############################################################

FxInfo
Flags InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	BackPackOffsets
		Type	Local
		At	Chest
		Geom	FX_Celestial_Backpack_Offsets
	End

	Event
		EName	BackPackOffsets_Small
		Type	Local
		At	Chest
		BhvrOverride
			Scale 0.75 0.75 0.75
		End
		Geom	FX_Celestial_Backpack_Offsets
	End

	Event
		EName	Offset13
		Type	Local
		At	BackPackOffsets
		AltPiv	13
	End

	Event
		EName	Offset13_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	13
	End

#############################################################

	Event
		EName	Offset01
		Type	Local
		At	BackPackOffsets
		AltPiv	1
		Part	:Fire_Highlights.part
		POther	Offset13
	End

	Event
		EName	Offset02
		Type	Local
		At	BackPackOffsets
		AltPiv	2
		Part	:Fire_Highlights.part
		POther	Offset01
	End

	Event
		EName	Offset03
		Type	Local
		At	BackPackOffsets
		AltPiv	3
		Part	:Fire_Highlights.part
		POther	Offset02
	End

	Event
		EName	Offset04
		Type	Local
		At	BackPackOffsets
		AltPiv	4
		Part	:Fire_Highlights.part
		POther	Offset03
	End

	Event
		EName	Offset05
		Type	Local
		At	BackPackOffsets
		AltPiv	5
		Part	:Fire_Highlights.part
		POther	Offset04
	End

	Event
		EName	Offset06
		Type	Local
		At	BackPackOffsets
		AltPiv	6
		Part	:Fire_Highlights.part
		POther	Offset05
	End

	Event
		EName	Offset07
		Type	Local
		At	BackPackOffsets
		AltPiv	7
		Part	:Fire_Highlights.part
		POther	Offset06
	End

	Event
		EName	Offset08
		Type	Local
		At	BackPackOffsets
		AltPiv	08
		Part	:Fire_Highlights.part
		POther	Offset07
	End

	Event
		EName	Offset09
		Type	Local
		At	BackPackOffsets
		AltPiv	09
		Part	:Fire_Highlights.part
		POther	Offset08
	End

	Event
		EName	Offset10
		Type	Local
		At	BackPackOffsets
		AltPiv	10
		Part	:Fire_Highlights.part
		POther	Offset09
	End

	Event
		EName	Offset11
		Type	Local
		At	BackPackOffsets
		AltPiv	11
		Part	:Fire_Highlights.part
		POther	Offset10
	End

	Event
		Type	Local
		At	BackPackOffsets
		AltPiv	12
		Part	:Fire_Highlights.part
		POther	Offset11
	End

#############################################################

	Event
		EName	Offset01_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	1
		Part	:Fire_Highlights.part
		POther	Offset13_Small
	End

	Event
		EName	Offset02_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	2
		Part	:Fire_Highlights.part
		POther	Offset01
	End

	Event
		EName	Offset03_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	3
		Part	:Fire_Highlights.part
		POther	Offset02
	End

	Event
		EName	Offset04_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	4
		Part	:Fire_Highlights.part
		POther	Offset03
	End

	Event
		EName	Offset05_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	5
		Part	:Fire_Highlights.part
		POther	Offset04
	End

	Event
		EName	Offset06_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	6
		Part	:Fire_Highlights.part
		POther	Offset05
	End

	Event
		EName	Offset07_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	7
		Part	:Fire_Highlights.part
		POther	Offset06
	End

	Event
		EName	Offset08_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	08
		Part	:Fire_Highlights.part
		POther	Offset07
	End

	Event
		EName	Offset09_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	09
		Part	:Fire_Highlights.part
		POther	Offset08
	End

	Event
		EName	Offset10_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	10
		Part	:Fire_Highlights.part
		POther	Offset09
	End

	Event
		EName	Offset11_Small
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	11
		Part	:Fire_Highlights.part
		POther	Offset10
	End

	Event
		Type	Local
		At	BackPackOffsets_Small
		AltPiv	12
		Part	:Fire_Highlights.part
		POther	Offset11
	End

#############################################################

End