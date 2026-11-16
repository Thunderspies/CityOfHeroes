#############################################################
## Thrown Confetti FX
#############################################################

FxInfo
Lifespan 120
#############################################################

Condition
	On	Time
	Time	0
	Event
		Ename 	GeoTube
		Type	Local
		At	WepR
		GEOM  	FX_Fireworks_Tube
		Lifespan 120
	End
End


Condition
	On	Time
	Time	35
	Random	1

	Event
		Ename 	Offset
		Type	Start
		At	Origin

		CHILDFX :Fireworks_Rocket_Yellow.fx
		Lifespan 30
	End

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Red.fx
		Lifespan 30
	End


	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Green.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	45
	Random	1

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Yellow.fx
		Lifespan 30
	End

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Red.fx
		Lifespan 30
	End


	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Green.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	70
	Random	1

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Yellow.fx
		Lifespan 30
	End

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Red.fx
		Lifespan 30
	End


	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Green.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	90
	Random	1

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Yellow.fx
		Lifespan 30
	End

	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Red.fx
		Lifespan 30
	End


	Event
		Ename 	Offset
		Type	Start
		At	Origin
		CHILDFX :Fireworks_Rocket_Green.fx
		Lifespan 30
	End
End


#############################################################

End