#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	Root
End


Lifespan	150

#############################
#############################################################################


Condition
	on	Time
	Time	20

	Event
		Type	local
		At	origin
		
		Sound alkaloidtoss 90 90 .85
	
	End

End




Condition
	on	Time
	Time	30

	Event
		Type	local
		At	origin
		
		Sound Poisongasressurect 90 90 .85
	
	End

End



Condition
	on	Time
	Time	20

	Event
		Type	local
		At	WepR
		
		Bhvr	:YellowGlassScaleDown.bhvr
		Geom	PoisonGlassBall
		
		LifeSpan	28
	End

End

Condition
	on	Time
	Time	35
	Event
		EName 	cylinderspawn
		Type	local
		At	T_Root
		Geom	CenterDummy
		BHVR	:PoisonHealRotate.bhvr
		#Splat	ember.tga
		LifeSpan	50
	End

End

Condition
	on	Time
	Time	45
	
	Event
		Type	local
		At	T_Root

		BHVR	Behaviors/GenericParticleFade.bhvr
		
		Part	:HealPoisonglowsUp.part

		Part	:HealPoisonUp.part
		Part	:HealPoisonSpecksUp.part
		Part	:HealPoisonSpecksUpYellow.part
		Part	:HealPoisonBubblesUp.part
		Part	:HealPoisonglowsUp.part
		Part	:HealPoisonUpDark.part

		LifeSpan	80

	End

	Event
		Type	local
		At	T_Root

		BHVR	Behaviors/GenericParticleFade.bhvr
		
		Part	:HealEnergyBase.part
		Part	:HealEnergyBaseFlat.part

		LifeSpan	70

	End

End

Condition
	On 	cycle
	Time	1

	Event
		EName	RotateNode
		Type	local
		At	cylinderspawn
		altpiv	1
		Bhvr	:PoisonHealRotate2.bhvr
		#Part	:HealEnergyUp.part
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time	1
	Chance	.6

	Event

		Type	start
		At	RotateNode
		
		#Bhvr	:PoisonHealRotate2.bhvr
		Part	:HealEnergyUp.part
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time	1
	Chance	.6

	Event

		Type	start
		At	RotateNode
		
		#Bhvr	:PoisonHealRotate2.bhvr
		Part	:HealEnergyUp.part
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time	1
	Chance	.6

	Event

		Type	start
		At	RotateNode
		
		#Bhvr	:PoisonHealRotate2.bhvr
		Part	:HealEnergyUp.part
		LifeSpan	10

	End

End

###########################################################################################
##Splats
##########################################################


Condition
	On 	Time
	Time	32

	Event
		EName 	sleeticesplatSpinsAndOffsets
		Type	start
		At	T_Root
		Geom	CenterDummy
		BHVR	:PoisonSplatRotate.bhvr
		#Splat	ember.tga
		LifeSpan	50
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets2
		Type	start
		At	T_Root
		Geom	CenterDummy
		BHVR	:PoisonSplatRotate.bhvr
		#Splat	ember.tga
		LifeSpan	50
	End
	
	Event
		EName 	sleeticesplatSpinsAndOffsets3
		Type	start
		At	T_Root
		Geom	CenterDummy
		BHVR	:PoisonSplatRotate.bhvr
		#Splat	ember.tga
		LifeSpan	50
	End

	Event
		EName 	sleeticesplatSpinsAndOffsets4
		Type	start
		At	T_Root
		Geom	CenterDummy
		BHVR	:PoisonSplatRotate.bhvr
		#Splat	ember.tga
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time	44

	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets
		Altpiv	1
		#sound	icebreak3 80 70 .96
		
		BHVR	:PoisonHealSplat.bhvr
		Splat	ember.tga
		LifeSpan	48
	End

End

Condition
	On 	Time
	Time	45
	
	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets2
		Altpiv	1
		
		BHVR	:PoisonHealSplat.bhvr
		Splat	ember.tga
		LifeSpan	48
	End
	
End	

Condition
	On 	Time
	Time	46
	
	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets4
		Altpiv	1
		
		BHVR	:PoisonHealSplat.bhvr
		Splat	ember.tga
		LifeSpan	48
	End
	
End

Condition
	On 	Time
	Time	47
	
	Event
		EName 	sleeticesplat
		Type	start
		At	sleeticesplatSpinsAndOffsets3
		Altpiv	1
		
		BHVR	:PoisonHealSplat.bhvr
		Splat	ember.tga
		LifeSpan	48
	End
	
End	


End		