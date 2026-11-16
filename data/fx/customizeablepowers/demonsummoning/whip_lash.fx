#############################################################
## Whip_Overhand.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	WhipAnim
		Type	Local
		At	WepR
		Bhvr	behaviors\FadeInOut.bhvr
		BhvrOverride
			TintGeom	1
		End
		Anim	CustomWeapon_Whip_Hellfire01
		Lifespan 50
	End

	Event
		EName	Whip_Hips
		Type	Local
		At	WhipAnim
		AnimPiv	Hips
	End

	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegL
	End

	Event
		EName	Whip_Hind_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegL
	End

	Event
		EName	Whip_Hind_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootL
	End

	Event
		EName	Whip_Hind_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeL
	End

	Event
		EName	Whip_Hind_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegR
	End

	Event
		EName	Whip_Hind_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegR
	End

	Event
		EName	Whip_Hind_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootR
	End

	Event
		EName	Whip_Hind_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeR
	End

	Event
		EName	Whip_Fore_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegL
	End

	Event
		EName	Whip_Fore_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegL
	End

	Event
		EName	Whip_Fore_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootL
	End

	Event
		EName	Whip_Fore_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeL
	End

	Event
		EName	Whip_Fore_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegR
	End

	Event
		EName	Whip_Fore_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegR
	End

	Event
		EName	Whip_Fore_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootR
	End

	Event
		EName	Whip_Fore_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeR
	End
End

##############################################################

Condition
	On	Time
	Time	8

	Event
		Type	PositOnly
		At	Whip_Hind_ULegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_LLegL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	PositOnly
		At	Whip_Hind_LLegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_FootL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	PositOnly
		At	Whip_Hind_FootL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_ToeL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	PositOnly
		At	Whip_Hind_ToeL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_ULegR
		Lifespan 7
	End
End


Condition
	On	Time
	Time	14

	Event
		Type	PositOnly
		At	Whip_Hind_ULegR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_LLegR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	PositOnly
		At	Whip_Hind_LLegR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_FootR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	PositOnly
		At	Whip_Hind_FootR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_ToeR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Whip_Hind_ToeR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Hind_ULegR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_FootL
		Lifespan 3
	End
End

Condition
	On	Time
	Time	26

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 3
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	PositOnly
		At	Whip_Fore_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_LLegR
		Lifespan 2
	End

	Event
		Type	PositOnly
		At	Whip_Fore_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_FootR
		Lifespan 2
	End
End
##############################################################################

Condition
	On	Time
	Time	13
	Event
		Type	Local
		At	WhipAnim
		AnimPiv	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Handle.part
		Part	:WhipCord_Glow.part
		POther	Whip_Hind_ULegL
		Lifespan 39 #37
	End
End

Condition
	On	Time
	Time	14
	Event
		Type	Local
		At	Whip_Hind_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_LLegL
		Lifespan 39 #37
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Whip_Hind_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_FootL
		Lifespan 39 #37

	End
End

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	Whip_Hind_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ToeL
		Lifespan 39 #35
	End



	Event
		Type	Local
		At	Whip_Hind_ToeL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ULegR
		Lifespan 39 #35

	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_LLegR
		Lifespan 37 #32

	End

	Event
		Type	Local
		At	Whip_Hind_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_FootR
		Lifespan 37 #32
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Whip_Hind_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ToeR
		Lifespan 34 #30
	End



	Event
		Type	Local
		At	Whip_Hind_ToeR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ULegL
		Lifespan 34 #30
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	Whip_Fore_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 32

	End



	Event
		Type	Local
		At	Whip_Fore_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		POther	Whip_Fore_FootL
		Lifespan 32 #13
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	Local
		At	Whip_Fore_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 28
	End

	Event
		Type	Local
		At	Whip_Fore_ToeL
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ULegR
		Lifespan 28
	End
End

Condition
	On	Time
	Time	26

	Event
		Type	Local
		At	Whip_Fore_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_LLegR
		Lifespan 26
	End



	Event
		Type	Local
		At	Whip_Fore_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_FootR
		Lifespan 26
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Whip_Fore_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		POther	Whip_Fore_ToeR
		Lifespan 24
	End
End

##############################################################

Condition
	On	Time
	Time	22


	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Fire_Trail.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 15
	End

	Event
		Type	PositOnly
		At	Whip_Fore_LLegR
		Part	:Whip_Fire_Trail.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 15
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	WhipCrackOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 5 8
		End
	End

	Event
		EName	WhipCrackOffset2
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		3 5 5
		End
	End
End

Condition
	On	Time
	Time	32

	Event
		Type	StartPositOnly
		At	WhipCrackOffset
		Part	:Whip_Crack_FlashB.part
		Part	:Whip_Crack_Flash.part
		Part	:Whip_Crack_Fire.part
		Part	:Whip_Crack_Smoke_Linger.part
		Part	:Whip_Sparks_Burst.part
		Lifespan 15
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	PositOnly
		At	WhipCrackOffset2
		Part	:Whip_Crack_FlashB.part
		Part	:Whip_Crack_Flash.part
		Part	:Whip_Crack_Fire.part
		Part	:Whip_Crack_Smoke_Linger.part
		Part	:Whip_Sparks_Burst.part
		Lifespan 15
	End
End
##############################################################

Condition
	On 	Time
	Time 	52

	Event
		Type	Local
		At	Whip_Hind_LLegL
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 6
	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
		Part	:Whip_Smoke.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 6
	End


	Event
		Type	Local
		At	Whip_Hind_LLegR
		Part	:Whip_Smoke.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 7
	End

	Event
		Type	Local
		At	Whip_Fore_ULegL
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 7
	End

	Event
		Type	Local
		At	Whip_Fore_LLegL
		Part	:Whip_Smoke.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Part	:Whip_Fire_Extinguish_Large.part
		Lifespan 8
	End

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part

		Lifespan 9
	End

End

Condition
	On	Time
	Time	53

	Event
		Type	PositOnly
		At	Whip_Fore_ULegR
		Part	:Whip_Sparks.part
		Lifespan 6
	End


	Event
		Type	PositOnly
		At	Whip_Fore_FootR
		Part	:Whip_Sparks.part
		Lifespan 6
	End
End
#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Origin
		Sound Whip_CrackWhip_01 80 80 1
	End
End

#############################################################

End