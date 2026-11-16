#############################################################
## ChildFX_Droplet_Falling.fx
#############################################################

FxInfo
Lifespan 60

#############################################################


Condition
	On	Time
	Time	0



	Event
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 40 0
		End
		Part	:Offset_WaterDroplet.part
		Part	:Offset_WaterDroplet_Group.part

		Lifespan 60

	End
End

Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	Origin
		Part	:Offset_Splash.part
		SoundNoRepeat 3
		Sound Drip_Light1 50 50 .15
		Sound Drip_Light2 50 50 .1
		Sound Drip_Light3 50 50 .15
		Sound Drip_Light4 50 50 .1
		Sound Drip_Light5 50 50 .15
		Sound Drip_Light6 50 50 .1
		Sound Drip_Light7 50 50 .15
		Lifespan 	1
	End
End