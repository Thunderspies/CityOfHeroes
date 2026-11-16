#############################################################
## Whip_Overhand.fx
#############################################################

FxInfo
LifeSpan 130

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
			FadeInLength	20
			FadeOutLength	5
			TintGeom	1
		End
		Anim	CustomWeapon_Whip_Hellfire01
		Lifespan 30
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

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	PositOnly
		At	WepR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		Lifespan 100
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	PositOnly
		At	Whip_Hind_LLegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_FootL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	PositOnly
		At	Whip_Hind_FootL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_FootL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	PositOnly
		At	Whip_Hind_ToeL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_ULegR
		Lifespan 7
	End
End


Condition
	On	Time
	Time	9

	Event
		Type	PositOnly
		At	Whip_Hind_ULegR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_LLegR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	11

	Event
		Type	PositOnly
		At	Whip_Hind_LLegR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_FootR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	PositOnly
		At	Whip_Hind_FootR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_ToeR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	PositOnly
		At	Whip_Hind_ToeR
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Hind_ToeR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Fore_LLegL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	19

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Fore_FootL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	21

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	:Whip_Fire_Stationary.part
		Part	:Whip_Fire_Stationary_Alpha.part
		Part	:Whip_Fire_Extinguish_Highlight.part
		POther	Whip_Fore_ToeL
		Lifespan 5
	End
End

#############################################################################

Condition
	On	Time
	Time	11
	Event
		Type	Local
		At	WhipAnim
		AnimPiv	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Handle.part
		Part	:WhipCord_Glow.part
		POther	Whip_Hind_ULegL
		Lifespan 90
	End

	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_LLegL
		Lifespan 7
	End


	Event
		EName	Whip_Hind_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_FootL
		Lifespan 7

	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Whip_Hind_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_ToeL
		Lifespan 7
	End



	Event
		Type	Local
		At	Whip_Hind_ToeL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_ULegR
		Lifespan 7

	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_LLegR
		Lifespan 8

	End

	Event
		Type	Local
		At	Whip_Hind_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_FootR
		Lifespan 8
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	Whip_Hind_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Hind_ToeR
		Lifespan 8
	End



	Event
		Type	Local
		At	Whip_Hind_ToeR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_ULegL
		Lifespan 8
	End
End

Condition
	On	Time
	Time	19

	Event
		Type	Local
		At	Whip_Fore_ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_LLegL
		Lifespan 9
	End



	Event
		Type	Local
		At	Whip_Fore_LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_FootL
		Lifespan 9
	End
End


Condition
	On	Time
	Time	21

	Event
		Type	Local
		At	Whip_Fore_FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_ToeL
		Lifespan 7
	End

	Event
		Type	Local
		At	Whip_Fore_ToeL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipCord_Glow_Buff.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_ULegR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	Whip_Fore_ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Lash.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_LLegR
		Lifespan 5
	End

	Event
		Type	Local
		At	Whip_Fore_LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_FootR
		Lifespan 5
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Whip_Fore_FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:WhipCord_Tip.part
		Part	:WhipFire_Buff.part
		Part	:WhipFire_Buff_Alpha.part
		POther	Whip_Fore_ToeR
		Lifespan 4
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Prime2
		Type	PositOnly
		At	WepR
		Bhvr	behaviors\WhipCord.bhvr
		BhvrOverride
			Alpha		255
			StartColor 	200 255 0
		End
		Geom	Tintable_XrayBeam2
		Magnet	T_Chest
		LookAt	T_Chest
		LifeSpan 60
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:WhipCord_Beam.part
		Part	:WhipCord_Glow_Beam.part
		Part	:WhipCord_Hellfire_Beam.part
		Part	:WhipCord_Hellfire_Beam_Alpha.part
		PMagnet	T_Chest
		LifeSpan 90
	End

	Event
		EName	Ring
		Type	Local
		At	T_Chest
		BhvrOverride
			Scale		0.85 0.4 0.5
			TintGeom	1
		End
		Geom	Tintable_MegaRing2
		LifeSpan	100

	End

	Event
		EName	Ring
		Type	Local
		At	T_Chest
		BhvrOverride
			Scale		0.85 0.4 0.5
			TintGeom	1
			PyrRotate 0 0 30
		End
		Geom	Tintable_MegaRing2
		LifeSpan	100

	End
End

Condition
	On	Cycle
	Time	20

	Event
		EName	Prime
		At 	WepR
		Type	StartPositOnly
		Bhvr	behaviors\BeanBagProjectile.bhvr
		BhvrOverride
			Gravity			.004
			TrackRate		2.0
		End
		Part	:Corruption_Fireball_Core.part
		Part	:Corruption_Fireball_Steam.part
		Part	:Corruption_Fireball_Residue_Alpha.part
		Part	:Corruption_Fireball_Residue_Highlight.part
		Part	:Corruption_Fireball_Residue.part
		POther	T_Chest
		Magnet	T_Chest
		Lookat	T_Chest
		Lifespan 80
	End


End
##############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	Origin
		Sound Whip_HellOnEarth_01 80 80 1
	End
End

#############################################################

End