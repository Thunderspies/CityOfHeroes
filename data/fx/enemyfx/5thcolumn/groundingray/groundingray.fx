#############################################################
## GroundingRay.fx
#############################################################

FxInfo

LifeSpan 165

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	GunTip
		Type	local
		At	WepR
		Bhvr	:GroundingRayGunTip.bhvr
		Sound GroundingRayFire_01 40 40 1
		Lifespan 90
	End
End

Condition
	On	Time
	Time	3

	Event

		Ename	ChargedBolt6
		Type	Local
		At	GunTip
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:GlowQuick_Body.part
		Part	:Embers.part
		Part	:Circle_Fill_Small.part
		Lifespan 90
	End
End

Condition
	On	Time
	Time	10

	Event

		Ename	ChargedBolt6
		Type	Local
		At	GunTip
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Beam.part
		Part	:Beam_Subtractive.part
		Part	:BeamCore.part
		Part	:SapperStream01.part
		Part	:SapperStream_Subtractive.part
		POther	T_Chest
		Lifespan 70
	End
End

#############################################################

End