#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	6
	
	
	Event
		Type	Local
		At	root
		geom	HeatWaveSphere
		bhvr	:Scale.bhvr
	End

End

Condition
	On	Time
	Time	6
	
	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
	End
End


Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Origin
		Sound firesword_loop 50 50 .22
	End
End


Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Origin
		Sound fireball1 50 50 .5
	End
End



Condition
	On	Time
	Time	20
	
	Event
		EName   RadiusFxScalex
		Type	local
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	:PlasmaShieldCloud01B.part
		Part2	:PlasmaShieldCloud02B.part
		Part4	:PlasmaShieldSparksB.part
		
		LifeSpan	30
		
	End
End

Condition
	On	Time
	Time	10
		
	Event
		EName	Pivot
		Type	Local
		At	head
		
		part	:PlasmaFireCore.part

	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		
		part	:PlasmaFireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		
		part	:PlasmaFireCore4.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		
		part	:PlasmaFireCoreTrail2.part
		Part	:PlasmaFireCoreSparks.part
	End
	
	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	:PlasmaFireCore1.part
		part	:PlasmaFireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	:PlasmaFireCore1.part
		part	:PlasmaFireCoreTrail.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part	:PlasmaFireCore1Feet.part
		part	:PlasmaFireCoreTrailFeet.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part	:PlasmaFireCore1Feet.part
		part	:PlasmaFireCoreTrailFeet.part
	End

End


################################################################################################

Condition
	On	Time
	Time	15
	
	Event
		EName	delay
		Type	Local
		At	root
		
	End

End

Condition

	On 	cycle
	Time 	60
	Chance	1

	Event
		EName   RadiusFxScale
		Type	local
		At	delay
		Bhvr	powers/firecontrol/FireShield.bhvr
		Sound aura6 50 50 .24
		Sound aura5 50 50 .22
		Sound aura7 50 50 .19
		LifeSpan	30
	End

#	Event
#		EName 	Prime1
#		Type	Local 
#		At	RadiusFxScale
#		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
#		Part1	:FireShieldCloud01B.part
#		Part2	:FireShieldCloud02B.part
#		Part4	:FireShieldSparksB.part
#		
#		LifeSpan	30
#		
#	End		

End


 #########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	6
	
	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
	End
End


Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Origin
		Sound firesword_loop 50 50 .22
	End
End


Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Origin
		Sound fireball1 50 50 .5
	End
End


#
#
#Condition
#	On	Time
#	Time	20
#	
#	Event
#		EName   RadiusFxScalex
#		Type	local
#		At	root
#		Bhvr	powers/firecontrol/FireShield.bhvr
#		LifeSpan	30
#	End
#
#	Event
#		EName 	Prime1
#		Type	Local 
#		At	RadiusFxScalex
#		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
#		Part1	:FireShieldCloud01B.part
#		Part2	:FireShieldCloud02B.part
#		Part4	:FireShieldSparksB.part
#		
#		LifeSpan	30
#		
#	End
#End

#Condition
#	On	Time
#	Time	10
#	
#	Event
#		EName   eclipse
#		Type	local
#		At	Hips
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#part1	:FireAuraOut.part
#		LifeSpan	30
#	End
#	
#	Event
#		EName	Pivot
#		Type	Local
#		At	head
#		
#		part2	:FireCore.part
#
#	End
#
#	Event
#		EName	Hips
#		Type	Local
#		At	Hips
#		
#		#part2	:FireCore2.part
#		part3	:FireCoreTrail1.part
#	End
#
#	Event
#		EName	Chest
#		Type	Local
#		At	back
#		
#		part2	:FireCore4.part
#	End
#
#	Event
#		EName	Chest
#		Type	Local
#		At	Chest
#		
#		#part2	:FireCore3.part
#		part3	:FireCoreTrail2.part
#		Part4	:FireCoreSparks.part
#	End
#	
#	Event
#		EName	Lleg
#		Type	Local
#		At	LlegL
#		part1	:FireCore1.part
#		part2	:FireCoreTrail.part
#	End
#
#	Event
#		EName	Rleg
#		Type	Local
#		At	LlegR
#		part1	:FireCore1.part
#		part2	:FireCoreTrail.part
#	End
#
#	Event
#		EName	Lleg
#		Type	Local
#		At	FootL
#		part1	:FireCore1Feet.part
#		part2	:FireCoreTrailFeet.part
#	End
#
#	Event
#		EName	Rleg
#		Type	Local
#		At	FootR
#		part1	:FireCore1Feet.part
#		part2	:FireCoreTrailFeet.part
#	End
#
#End
#
#
#################################################################################################
#
#Condition
#	On	Time
#	Time	15
#	
#	Event
#		EName	delay
#		Type	Local
#		At	root
#		
#	End
#
#End
#
#Condition
#
#	On 	cycle
#	Time 	60
#	Chance	1
#
#	Event
#		EName   eclipse
#		Type	local
#		At	Hips
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#part1	:FireAuraOut.part
#		LifeSpan	30
#	End
#
#	Event
#		EName   RadiusFxScale
#		Type	local
#		At	delay
#		Bhvr	powers/firecontrol/FireShield.bhvr
#		Sound	aura6 50 30 .24
#		Sound	aura5 50 30 .22
#		Sound	aura7 50 30 .19
#		LifeSpan	30
#	End
#
##	Event
##		EName 	Prime1
##		Type	Local 
##		At	RadiusFxScale
##		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
##		Part1	:FireShieldCloud01B.part
##		Part2	:FireShieldCloud02B.part
##		Part4	:FireShieldSparksB.part
##		
##		LifeSpan	30
##		
##	End		
#
#End
#
#End				

End				