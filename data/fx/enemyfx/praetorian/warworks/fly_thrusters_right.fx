#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 250

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Plume.part
		Lifespan 50
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Plume_Left.part
		Lifespan 50
	End


	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Jet.part
		Part	:Spray_Plasma_Body.part
		Lifespan 55
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Jet_Left.part
		Part	:Spray_Plasma_Body_Left.part
		Lifespan 55
	End
End

Condition
	On	Time
	Time	120

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Smoke.part
		Lifespan 45
		Lifespan 10
	End


	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Spray_Plasma_Smoke_Left.part
		Lifespan 45
		Lifespan 10
	End
End

#############################################################

End