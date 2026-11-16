#############################################################
## AbyssalMending_Hit.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Root
		Sound Demon_AbyssalRecon 30 30 0.5
		Part	:Heal_Shockwave_Big.part
		Part	:Buff_Shockwave_Smoke_Big.part
		Part	:Buff_Shockwave_Smoke_Big2.part
		Lifespan 60
	End

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors\powers\firecontrol\Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.2 0
		End
		Part	:ShieldOfHellfire_SmokeRing.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Spinner
		Type	PositOnly
		At	Root
		BhvrOverride
			Spin	0 0.015 0
		End
	End

	Event
		Type	Local
		At	Spinner
		Part	:Globe_Sparks.part
		Part	:Globe_Cinders.part
	End
End

#############################################################

End


