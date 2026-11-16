#########################################################
## Staff of the M.A.G.I. - Ice Burst (PBAOE)
#########################################################

FxInfo

LifeSpan	90

## CAST FX ###############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StaffEnd
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 2
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	IceBlock
		Type	Local 
		At	StaffEnd
		Bhvr 	behaviors/powers/coldcontrol/icearmor.bhvr
		BhvrOverride
			Scale			0.5 0.5 0.75
			ScaleRate		1.0 1.0 1.0
			EndScale		0.5 0.5 0.75
			FadeInLength		20
			FadeOutLength		0
			Alpha			128
			PyrRotate		0 90 0
		End
		geom	FX_IceArmor_Chest
		part2	POWERS\ColdControl\IceArmorBits.part
		part3	POWERS\ColdControl\IceArmorBitsSmall.part
		part4	POWERS\ColdControl\Snowfall.part
		Lifespan	25
	End
End

Condition
	On 	Time
	Time 	0
	
	Event	
		ENAME	Glow
		Type	Local
		At	StaffEnd
		part4	:IceBurst_Streak01.part
		Lifespan	25
	End
End

## BURST ###############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Sound
		Type	Local
		At	StaffEnd
		Sound Ice8 100.0 100.0 1.0
	End

	Event
		EName	Flash1
		Type	Local
		At	StaffEnd
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		LifeSpan	27
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName   Flash1
		Type	Local
		At	StaffEnd
		Part1	POWERS\ColdControl\IceBoltSnowSkatter.part
	End

	Event
		EName	Ring1
		Type	start
		At	StaffEnd
		
		Part2	:IceBurst_Ring01.part
		Lifespan	25
	End

	Event
		EName	Ring1
		Type	start
		At	StaffEnd
		
		Part2	:IceBurst_Spikes01.part
		Lifespan	4
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName   Flash
		Type	Local
		At	StaffEnd
		Part1	POWERS\ColdControl\IceBoltHandFlash.part
		Part2	POWERS\ColdControl\IceBoltHandFlash2.part
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName   IceSpikes
		Type	Local
		At	StaffEnd
		#Part1	POWERS\ColdControl\IceBoltHandFlash.part
		#Part2	POWERS\ColdControl\IceBoltHandFlash2.part
	End
End

## PHYSICS PIECES ###############################################################

Condition
	On 	Time
	Time	25

	Repeat 4
	
	Event
		Ename	BlockofIceExplosionNew1
		Type	local 
		At	StaffEnd
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	V_COV\PhysicsEnabled\IceCubesExplode.fx
		Lifespan	60
		LifespanJitter	30
	End

	Event
		Ename	BlockofIceExplosionNew1
		Type	local 
		At	StaffEnd
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx
		Lifespan	60
		LifespanJitter	30
	End
	
	Event
		HardwareOnly
		Ename	BlockofIceExplosionNew1
		Type	local 
		At	StaffEnd
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	V_COV\PhysicsEnabled\IceChunkExplode.fx
		Lifespan	60
		LifespanJitter	30
	End

		
	Event
		HardwareOnly
		Ename	BlockofIceExplosionNew1
		Type	local 
		At	StaffEnd
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatter.fx
		Lifespan	60
		LifespanJitter	30
	End
End

#################################################################

End