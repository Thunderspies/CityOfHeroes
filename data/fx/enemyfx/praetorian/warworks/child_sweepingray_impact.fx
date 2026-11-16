#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	:Hit_Flash_Large.part
		Part	:Hit_Shockwave.part
	End

	Event
		EName	FireAnchor
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0 -1 0
		End
	End

	Event
		Type	PositOnly
		At	Origin
		Part	:Ray_Impact_Smoke.part
		Part	:Ray_Impact_Smoke_Lingering.part

		Lifespan 30
	End

	Event
		Type	Local
		At	Origin
		Part	:Ray_Impact_Glow_Core.part
		Part	:Ray_Impact_Tendril.part
		Part	:Ray_Impact_Sparks.part
		Part	:Ray_Impact_Glow_Lingering.part

		Lifespan 30
	End

	Event
		Type	Positonly
		At	Origin
		BhvrOverride
			PyrRotate	200 0 45
		End

		Part	:Ray_Impact_Glow_Flat.part

		Lifespan 30
	End
End

Condition
	On	Cycle
	Time	2

	Event
		Type	PositOnly
		At	FireAnchor
		ChildFX	:Child_Ray_Impact_Fire.fx

		Lifespan 60
	End
End
#############################################################

End