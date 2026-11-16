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
		Anim	CustomWeapon_Whip_Hellfire01
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
		EName	Whip_Hind_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegR
	End

	Event
		EName	Whip_Hind_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegR
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
	Time	0

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
	Time	1

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
	Time	2

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
	Time	4

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
	Time	6

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
	Time	8

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
	Time	10

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
	Time	12

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
	Time	14

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_FootL
		Lifespan 4
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 4
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Whip_Fore_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_LLegR
		Lifespan 3
	End

	Event
		Type	PositOnly
		At	Whip_Fore_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_FootR
		Lifespan 3
	End
End
#################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WhipAnim
		AnimPiv	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Handle.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipCord_Glow_Continuous.part
		POther	Whip_Hind_ULegL
		Lifespan 26
	End

	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	Whip_Hind_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_LLegL
		Lifespan 26
	End


	Event
		Type	Local
		At	Whip_Hind_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_FootL
		Lifespan 26

	End
End

Condition
	On	Time
	Time	11

	Event
		Type	Local
		At	Whip_Hind_FootL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ToeL
		Lifespan 26
	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ULegR
		Lifespan 26

	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Whip_Hind_ULegR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_LLegR
		Lifespan 26
	End

	Event
		Type	Local
		At	Whip_Hind_LLegR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_FootR
		Lifespan 26
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Whip_Hind_FootR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ToeR
		Lifespan 24
	End



	Event
		Type	Local
		At	Whip_Hind_ToeR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ULegL
		Lifespan 24
	End
End

Condition
	On	Time
	Time	14

	Event
		Type	Local
		At	Whip_Fore_ULegL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 24
	End



	Event
		Type	Local
		At	Whip_Fore_LLegL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_FootL
		Lifespan 24
	End
End


Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Whip_Fore_FootL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 24
	End



	Event
		Type	Local
		At	Whip_Fore_ToeL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ULegR
		Lifespan 24
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	Whip_Fore_ULegR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Glow_Long.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_LLegR
		Lifespan 23
	End



	Event
		Type	Local
		At	Whip_Fore_LLegR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		POther	Whip_Fore_FootR
		Lifespan 23
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	Whip_Fore_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		POther	Whip_Fore_ToeR
		Lifespan 28
	End

End

#################################################################

Condition
	On	Time
	Time	0

	Event
		EName	WhipCrackOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 8 8
		End
	End
End

Condition
	On	Time
	Time	28

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
	Time	30

	Event
		Type	Local
		At	Whip_Hips
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Lifespan 10
	End

	Event
		Type	Local
		At	Whip_Hind_LLegL
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
		Part	:Whip_Smoke.part
		Lifespan 5
	End


	Event
		Type	PositOnly
		At	Whip_Hind_LLegR
		Part	:Whip_Smoke.part
		Part	:Whip_Fire_Extinguish_Large.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 7
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	:Whip_Smoke.part
		Part	:Whip_Smoke_Linger.part
		Lifespan 7
	End

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Smoke.part
		Part	:Whip_Fire_Extinguish.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Extinguish_Alpha.part
		Lifespan 7
	End

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Sparks.part
		Part	:Whip_Fire_Extinguish_Large.part
		Lifespan 5
	End
End



Condition
	On	Time
	Time	30

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Sparks.part
		Lifespan 10
	End
End


#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	Origin
		Sound Whip_Lash_01 80 80 1
	End
End

#############################################################

End