#############################################################
## Absorption.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	Origin
		SOund	IcePunch1 80 30 .9
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Origin
		SOund	coldblast2 80 30 .8
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionLegBits.part
		part3	Powers\ColdControl\AbsorptionLegBitsSmall.part
		Sound IceArmor_loop 80 80 .66
		geom	FX_IceArmor_LLEGL
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionLegBits.part
		part3	Powers\ColdControl\AbsorptionLegBitsSmall.part
		geom	FX_IceArmor_LLEGR
	End

	Event
		Type	Local
		At	FootL
		part2	Powers\ColdControl\AbsorptionLegBits.part
		part3	Powers\ColdControl\AbsorptionLegBitsSmall.part
	End

	Event
		Type	Local
		At	FootR
		part2	Powers\ColdControl\AbsorptionLegBits.part
		part3	Powers\ColdControl\AbsorptionLegBitsSmall.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
		geom	FX_IceArmor_ULEGR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
		geom	FX_IceArmor_ULEGL
	End

	Event
		Type	Local
		At	WepL
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
	End

		Event
		Type	Local
		At	WepR
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
	End
End

Condition
	On 	Time
	Time 	20

		Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
		geom	FX_IceArmor_LARMR
	End

		Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
		geom	FX_IceArmor_LARML
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	Hips
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		geom	FX_IceArmor_HIPS
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		geom	FX_IceArmor_UARML
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		geom	FX_IceArmor_UARMR
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	Head
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		part2	Powers\ColdControl\AbsorptionBits.part
		part3	Powers\ColdControl\AbsorptionBitsSmall.part
		geom	FX_IceArmor_HEAD
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors\powers\coldcontrol\icearmorTransparrent.bhvr
		geom	FX_IceArmor_CHEST
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	30
	Chance	.2

	Event
		Bhvr	behaviors\genericParticleFade.bhvr
		Type	Local
		At	LLEGL
		part1	powers\invulnerability\FortitudeRaysSmall.part
		part2	powers\invulnerability\FortitudeRing.part
		part3	powers\invulnerability\FortitudeRing.part
		part4	powers\invulnerability\FortitudeDots.part
		LifeSpan 33
	End
End

Condition
	On 	Cycle
	Time 	33
	Chance	.2

	Event
		Bhvr	behaviors\genericParticleFade.bhvr
		Type	Local
		At	LLEGR
		part1	powers\invulnerability\FortitudeRaysSmall.part
		part2	powers\invulnerability\FortitudeRing.part
		part3	powers\invulnerability\FortitudeRing.part
		part4	powers\invulnerability\FortitudeDots.part
		LifeSpan 33
	End
End

Condition
	On 	Cycle
	Time 	35
	Chance	.2

	Event
		Bhvr	behaviors\genericParticleFade.bhvr
		Type	Local
		At	UArmL
		part1	powers\invulnerability\FortitudeRaysSmall.part
		part2	powers\invulnerability\FortitudeRing.part
		part3	powers\invulnerability\FortitudeRing.part
		part4	powers\invulnerability\FortitudeDots.part
		LifeSpan 33
	End
End

Condition
	On 	Cycle
	Time 	27
	Chance	.2

	Event
		Bhvr	behaviors\genericParticleFade.bhvr
		Type	Local
		At	UArmR
		part1	powers\invulnerability\FortitudeRaysSmall.part
		part2	powers\invulnerability\FortitudeRing.part
		part3	powers\invulnerability\FortitudeRing.part
		part4	powers\invulnerability\FortitudeDots.part
		LifeSpan 33
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWind.bhvr
		part1	Powers\ColdControl\IceMistUpwards.part
		part2	Powers\ColdControl\SnowMistUpwards.part
		geom	FX_TmpCtrlWisp01
		Lifespan 26
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWindFast.bhvr
		geom	FX_TmpCtrlWisp01
		Lifespan 24
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	start
		At	Root
		Bhvr 	powers\coldcontrol\IceArmorWindFirst.bhvr
		geom	FX_TmpCtrlWisp01
		Lifespan 24
	End
End

#############################################################

End