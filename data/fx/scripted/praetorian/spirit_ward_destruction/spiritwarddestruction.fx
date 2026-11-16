#############################################################
## Overload_Attack.fx
#############################################################

FxInfo
Lifespan 700

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Head
		Bhvr	behaviors/CameraShake01_Strong.bhvr
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			FadeOutLength	1
		End
		part	:Overload_Flash_Big.part
		Part	:Overload_Streaks_Windup.part
		Part	:Flash_LightRays.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	Start
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			FadeOutLength	1
		End
		Flags	AdoptParentEntity
		part	:Overload_Flash_Initial.part
		part	:Overload_Flash_Big.part
		part	:Overload_Streaks_Big.part
		part	:Overload_Streaks_Big_Alpha.part
		part	:Overload_Streaks_BigB.part
		part	:Overload_Streaks_BigB_Alpha.part
		Part	:Overload_Sparks_Big.part
		Part	:Overload_Streaks_Big_Highlight.part
		Lifespan 5
	End

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			FadeOutLength	1
		End
		CHILDFX	Explosions\WoodExplosion\DarkWoodExplosion_Small.fx
		Lifespan 200
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			FadeOutLength	1
		End
		Part	:Overload_Shockwave_Big.part
		Part	:Overload_Shockwave_Broken.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Start
		At	Head
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			FadeOutLength	1
		End
		Part	:Overload_Smoke_Additive.part
		Part	:Overload_Smoke_Big.part
		Part	:Overload_Electric.part
		Lifespan 10
	End
End

#############################################################

End