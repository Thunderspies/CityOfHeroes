#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo
Lifespan 100

#########################################################

Condition
	On 	Time
	Time	35
	Event
		Type	Local
		At	Root
		Sound 	Fireworks_Launch_01 100 100 .8
		Sound 	Fireworks_Launch_02 100 100 .8
		Sound 	Fireworks_Launch_03 100 100 .8
	End
End
Condition
	On 	Time
	Time	48
	Event
		Type	Local
		At	Root
		Sound 	Fireworks_Sparkle_01 100 100 .8
		Sound 	Fireworks_Sparkle_02 100 100 .8
		Sound 	Fireworks_Sparkle_03 100 100 .8
	End
End

Condition
	On	Time
	Time	0
	Event
		Ename 	GeoTube
		Type	Local
		At	WepR
		GEOM  	FX_Fireworks_Tube
		Lifespan 100
	End
End

Condition
	On	Time
	Time	35
	Random	1

	Event
		Ename 	SparkleYellow
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Sparkle_Yellow.fx
		Lifespan 100
	End

	Event
		Ename 	SparkleRed
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Sparkle_Red.fx
		Lifespan 100
	End


	Event
		Ename 	SparkleGreen
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Sparkle_Green.fx
		Lifespan 100
	End
End

#############################################################

End