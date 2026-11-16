#############################################################
## Whip_Strike.fx
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

#############################################################

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
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 6
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_FootL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	26

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 4
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
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Part	:Whip_Fire_Stationary_Alpha.part
		POther	Whip_Fore_FootR
		Lifespan 2
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WhipAnim
		AnimPiv	Hips
		Part	:WhipCord_Handle.part
		Part	:WhipCord_Glow_Continuous.part
		POther	Whip_Hind_ULegL
		Lifespan 31
	End

	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv Hind_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_LLegL
		Lifespan 31
	End

	Event
		EName	Whip_Hind_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_FootL
		Lifespan 31
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Whip_Hind_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ToeL
		Lifespan 28

	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ULegR
		Lifespan 28
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_LLegR
		Lifespan 22

	End

	Event
		Type	Local
		At	Whip_Hind_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_FootR
		Lifespan 22
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	Whip_Hind_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Hind_ToeR
		Lifespan 19
	End

	Event
		Type	Local
		At	Whip_Hind_ToeR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ULegL
		Lifespan 19
	End
End

Condition
	On	Time
	Time	32

	Event
		Type	Local
		At	Whip_Fore_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 16
	End

	Event
		Type	Local
		At	Whip_Fore_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_FootL
		Lifespan 16
	End
End

Condition
	On	Time
	Time	36

	Event
		Type	Local
		At	Whip_Fore_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Fore_ToeL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipCord_Glow_Continuous.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ULegR
		Lifespan 13
	End
End

Condition
	On	Time
	Time	37

	Event
		Type	Local
		At	Whip_Fore_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_LLegR
		Lifespan 11
	End

	Event
		Type	Local
		At	Whip_Fore_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_FootR
		Lifespan 11
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	Local
		At	Whip_Fore_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		Part	:WhipFire_Overhand.part
		Part	:WhipFire_Overhand_Highlight.part
		Part	:WhipFire_Overhand_Alpha.part
		POther	Whip_Fore_ToeR
		Lifespan 4
	End
End

Condition
	On	Time
	Time	39

	Event
		EName	Prime
		At 	Whip_Fore_ToeL
		Type	Start
		Bhvr	behaviors\BeanBagProjectile.bhvr
		BhvrOverride
			PositionOffset		0 -2 0
			InitialVelocity		0 0 0
			TrackRate		3.0
		End
		Part	:Corrupution_Projectile.part
		Part	:Corrupution_Projectile_Highlight.part
		Magnet	Target
		Lookat	Target
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Fire_Trail.part
		Lifespan 12
	End
End

Condition
	On	Time
	Time	39

	Event
		Type	PositOnly
		At	Whip_Fore_ToeR
		Part	:Whip_Sparks.part
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Fire_Extinguish.part
		Lifespan 10
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	:Whip_Smoke.part
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish_Large.part
		Lifespan 8
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ToeL
		Part	:Whip_Smoke.part
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Sparks.part
		Lifespan 10
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ULegR
		Part	:Whip_Smoke.part
		Part	:Whip_Smoke_Linger.part
		Part	:Whip_Fire_Extinguish_Large.part
		Lifespan 3
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	Origin
		Sound Whip_Corruption_01 80 80 1
	End
End

#############################################################

End