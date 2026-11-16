#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 60

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_HypnoThorn_01 200 100 .8
	End

	Event
		Type	local
		At	Hips
		Part	:Hypno_Dust_center.part
		Lifespan 60
	End
End


Condition
	On 	Time
	Time 	5


	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
		#Flags	AdoptParentEntity
		BhvrOverride
			FadeOutLength	10
			PositionOffset	0 0.4 0.6
		End
		part	:CoreExplosion_Flash_Initial.part
		part	:CoreExplosion_Flash_Big.part
		#part	:CoreExplosion_Streaks_Big.part
		#part	:CoreExplosion_Streaks_Big_Alpha.part
		part	:CoreExplosion_Streaks_BigB.part
		part	:CoreExplosion_Streaks_BigB_Alpha.part
		Part	:CoreExplosion_Shockwave_Big.part
		Lifespan 1
	End




	Event
		EName   RadiusFxScale
		Type	local
		At	Hips
		Bhvr	powers/firecontrol/Inferno.bhvr
		Part	:Hypno_Dust_Streaks.part
		Lifespan 60
	End


	Event
		EName 	Prime1
		Type	StartPositOnly
		At	RadiusFxScale
		bhvr    behaviors/Braz/ExpandRingTo60.bhvr
		BhvrOverride
			ScaleRate		0.5 0.5 0.5
 			EndScale		18 18 18

			FadeOutLength 100
		End
		Part1	:HypnoThorns.part
		Part	:Hypno_Dust.part
		Part	:Hypno_Dust2.part
		Part	:Hypno_Dust_Hilight.part
		Lifespan 60
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End





#############################################################

End