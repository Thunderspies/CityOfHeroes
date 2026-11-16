#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Gas_Fullscreen 100 100 .8
	End

	Event
		EName	AshAnchor
		Type	StartPositOnly
		CAMERASPACE
		Flags	OneAtATime

		Part	:Gas_FullScreen.part
		Part	:Gas_FullScreen_Hilight.part

		Lifespan 180
	End
End

Condition
	On	Time
	Time	60

	Event
		EName	AshAnchor
		Type	StartPositOnly
		CAMERASPACE
		Flags	OneAtATime

		Part	:Gas_FullScreen_Dark.part

		Lifespan 120
	End
End

#############################################################

End


