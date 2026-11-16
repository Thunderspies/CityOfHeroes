#############################################################
## AbyssalReconstruction.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Condition

	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Root
		Sound Demon_AbyssalRecon 100 100 .9
		Part	:Heal_Shockwave_Big.part
		Part	:Heal_Shockwave_Smoke_Big.part
		Part	:AbyssalReconstruction_Sparks.part
		Lifespan 60
	End

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.2 0
		End
		Part	:AbyssalMend_FireRing_Flat_Short.part
		Part	:AbyssalReconstruction_SmokeRing.part
		Part	:AbyssalMend_FireRing.part
		Lifespan 60
	End
End

Condition

	On 	Time
	Time 	13

	Event
		Type	PositOnly
		At	Root
		Part	:Heal_Shockwave_Big.part
		Part	:Heal_Shockwave_Smoke_Big.part
		Part	:AbyssalReconstruction_Sparks.part
		Lifespan 60
	End

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.2 0
		End
		Part	:AbyssalMend_FireRing_Flat_Short.part
		Part	:AbyssalReconstruction_SmokeRing.part
		Part	:AbyssalMend_FireRing.part
		Lifespan 60
	End
End

Condition

	On 	Time
	Time 	26

	Event
		Type	PositOnly
		At	Root
		Part	:Heal_Shockwave_Big.part
		Part	:Heal_Shockwave_Smoke_Big.part
		Part	:AbyssalReconstruction_Sparks.part
		Lifespan 60
	End

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.2 0
		End
		Part	:AbyssalMend_FireRing_Flat_Short.part
		Part	:AbyssalReconstruction_SmokeRing.part
		Part	:AbyssalMend_FireRing.part
		Lifespan 60
	End
End
#############################################################

End


