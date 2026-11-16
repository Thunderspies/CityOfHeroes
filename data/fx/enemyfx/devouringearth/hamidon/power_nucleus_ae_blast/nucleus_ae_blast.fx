#########################################################
## Hamidon - Electorlytic Blast
#########################################################

FxInfo
LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Waist
		Part1	:Devouring_Hamidon_Burst.part
		Part1	:Burst_Bits_01.part
	End
End

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	waist
		Sound mitoblast1 200 200 1.0
		Part1	:Burst_Beams_01.part

		Lifespan	40
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName	InitialSwordFire
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName 	Origin
		Type	Start 
		At	Waist
		Part1	:Devouring_Hamidon_Burst_02.part
		Part1	:Devouring_Hamidon_Burst_Core.part
		Lifespan	5
	End
End

#########################################################
## Tracking Projectile
#########################################################

Condition
	On 	Time
	Time 	70

	Event
		EName 	Prime
		Type	Start 
		At	Waist
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		BhvrOverride
			TrackRate		4
			TrackMethod		2
		End
		Magnet	Target
		LookAt	Target
		Part2	:Nucleus_Attack_Head_Inner.part
		Part3	:Nucleus_Attack_Body.part
		Part4	:Nucleus_Attack_Squiggle.part
		Part5	:Nucleus_Attack_Mist.part
		Part5	:Nucleus_Attack_Beams.part
		Part1	:Nucleus_Attack_Head.part
	End
End

Condition
	On	PrimeHit	

	Event
		Ename	NucleusAttack
		Type	Destroy
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

#########################################################
## AREA EFFECT EXPLOSION
#########################################################

Condition

	On	PrimeHit 

	Event
		EName 	Hookup
		Type	Local 
		At	T_Root
		Geom	RootToChestAdjustment
		Sound mitohit5 60 60 .75
		
	End

	Event
		EName 	Prime45
		Type	Local 
		At	T_Hips
		Part	:Hit_FX\Death_Flash_01.part
		Lifespan	7
		
	End	

	Event
		Type	Local 
		At	T_Root
		Part	:Hit_FX\Death_Flash_02.part
		Lifespan	7
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	hookup
		altpiv	1
		Bhvr	powers/firecontrol/Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:Hit_FX\Burst_Cloud_01.part
		Part2	:Hit_FX\Burst_Cloud_02.part
		Part3	:Hit_FX\IrradiateRings.part
		Part4	:Hit_FX\IrradiateGlows.part
		Part5	:Hit_FX\Outward_Ions.part
		Part1   :Hit_FX\Burst_Ring_Small.part
		Part2   :Hit_FX\Burst_Ring_Big.part		
	End		

#	Event
#		EName 	Prime8
#		Type	Local 
#		At	Hookup
#		Altpiv	1
#		Part1   :Hit_FX\Burst_Ring_Small.part
#		Part2   :Hit_FX\Burst_Ring_Big.part
#		Part1	:Hit_FX\Burst_Cloud_01.part
#		Part2	:Hit_FX\Burst_Cloud_02.part
#		Part3	:Hit_FX\IrradiateRings.part
#		Part4	:Hit_FX\IrradiateGlows.part
#		Part5	:Hit_FX\Outward_Ions.part
#
#	End

	Event
		EName 	Prime44
		Type	Local 
		At	Hips
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:Hit_FX\Inward_Ions.part
		Lifespan	60
	End	

	Event
		EName 	Prime44
		Type	Local 
		At	Hips
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part2	:Hit_FX\Death_Beams_01.part
		Lifespan	30
	End	

	Event
		EName 	rays
		Type	Destroy
	End

End

#########################################################
## TARGET HIT GLOW
#########################################################

Lifespan	160

Condition

	On	PrimeHit

	Event
		EName	1
		Type	Local
		At	T_chest
		#Geom	FX_ancestor
		Sound Glow8_loop 70 70 .82
	End
	
	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	:Hit_FX\HealingFlames.part
	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	:Hit_FX\HealingFlames.part
		PMagnet LLEGL
	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	:Hit_FX\HealingFlames.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	T_FootL
		part1	:Hit_FX\HealingFlames.part
	End

	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	:Hit_FX\HealingFlames.part
		PMagnet LlegR
	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	:Hit_FX\HealingFlames.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	T_Head
		part2	:Hit_FX\HealingFlames.part
	End

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part1	:Hit_FX\HealingFlames.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part1	:Hit_FX\HealingFlames.part
		PMagnet WepL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	:Hit_FX\HealingFlames.part
	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part1	:Hit_FX\HealingFlames.part
		PMagnet LArmR
	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part1	:Hit_FX\HealingFlames.part
		PMagnet WepR
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	:Hit_FX\HealingFlames.part
	End

	Event
		EName	15
		Type	Local
		At	T_Chest
		part1	:Hit_FX\HealingFlames.part
	End
End


End		