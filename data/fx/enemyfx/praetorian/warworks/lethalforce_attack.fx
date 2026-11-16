#############################################################
## LethalForce_Attack.fx
#############################################################

FxInfo
LifeSpan 250

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		Sound Plasma_Spray 200 200 1
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	3.1 0.7 0.0
		End
		Part	:Muzzle_Plasma_Plume_Large.part
		Part	:Muzzle_Glow_Large.part
		Part	:Muzzle_Rings_Faint_Large.part
		Part	:Muzzle_Tendril_Fast_Large.part
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	90


	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Plume_Large.part
		Lifespan 50
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Jet_Large.part
		Part	:Spray_Plasma_Body_Large.part
		Lifespan 50
	End
End

#Condition
#	On	Time
#	Time	120
#
#	Event
#		Type	Local
#		At	LArmR
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	2.4 0.7 0.0
#		End
#		Part	:Spray_Plasma_Smoke.part
#		Lifespan 45
#		Lifespan 10
#	End
#End

#############################################################

End