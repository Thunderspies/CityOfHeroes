#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo

Lifespan 250

#############################################################

Condition
	On Time
	Time 5
	Event
		Type	Local
		At	Root
		Sound 	Explosion_Fullscreen 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	flash
		Type	StartPositOnly
		CAMERASPACE
		Flags	OneAtATime
		Part	:Flash.part

		Lifespan 120
	End
End


Condition
	On	Time
	Time	15

	Event
		EName	Holocaust
		Type	StartPositOnly
		CAMERASPACE
		Part	:CAMERA_Debris.part
		Part	:CAMERA_Dark_Clouds.part
		Part	:CAMERA_Dust.part
		Lifespan 25
	End
End

Condition
	On	Time
	Time	25

	Event
		EName	Holocaust
		Type	StartPositOnly
		CAMERASPACE
		Part	:Explosion_FullScreen.part

		Lifespan 60
	End

	Event
		EName	Holocaust
		Type	StartPositOnly
		CAMERASPACE
		Part	:Explosion_FullScreen_Hilight.part

		Lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	blah
		Type	StartPositOnly
		CAMERASPACE

		Lifespan 25
	End
End

Condition
	On	Time
	time	70

	Event
		Ename	Dust
		Type	Start
		CAMERASPACE
		Part	:Explosion_FullScreen_Smoke.part
		Lifespan 40
	End
End


#############################################################

End


