#############################################################
## SteamPack_Impact.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake01Subtle.bhvr
		Lifespan 10
	End

	Event
		Type	Start
		At	Root
		Bhvr	:Dust_Expanding.bhvr
		BhvrOverride
			PositionOffset	0 1.0 0.0
		End
		Part	:Impact_Dust.part
		Part	:Impact_Dust_Lingering.part
		Lifespan 80
	End
End

#############################################################

End