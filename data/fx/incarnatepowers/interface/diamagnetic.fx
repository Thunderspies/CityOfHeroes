#############################################################
## GCCrushContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname Hips
End

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Chest
		part	:Flash.part
		Lifespan 5
	End

End

Condition

	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Diamagnetic_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 150

	End

	Event
		ENAME	Vortex
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		part	:GoldenCenter.part
		part	:MagnetPoles.part
		part	:Flaring_Streaks.part
		part	:DotsAway.part
		Pmagnet Hips
		#Sound glow6_loop 80 80 .85
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 1 0
		End
		part	:Lingering_Cloud.part
		part	:Glow.part
		Pmagnet Hips
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		part	:DotsAway_Ground.part
		PMagnet	Hips
	End
#
#	Event
#		Type	Local
#		At	Hips
#		BhvrOverride
#			PositionOffset	0 -2 0
#		End
#		part	:DotsIn.part
#	End

#	Event
#		Ename	GravitationalForce
#		Type	Local
#		At	Chest
#
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		10
#			PhysForcePower		60
#			PhysForcePowerJitter	10
#		End
#	End




End


#############################################################

End

