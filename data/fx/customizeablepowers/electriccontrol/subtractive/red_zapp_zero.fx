#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	10  ##50 (This need to be quick -steve)

########################################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Part1	:Red_ElectricityControl01white.part
		Part2	:Red_ElectricityControl02white.part
		Sound elecwindup_loop 80 80 .5
		
		

	End

	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Part1	:Red_ElectricityControl01white.part
		Part2	:Red_ElectricityControl02white.part
		
	End

End

Condition
	On 	Time
	Time 	5
	
	Event
		Ename	Prime
		Type	start
		At	HandR

		Bhvr	Behaviors/Projectile4.bhvr

		magnet Target	
	End		
	
	Event
		Type	start
		At	HandR

		Part1	:Red_ChargedBolt06b.part
		Part2	:Red_ChargedBolt05b.part
		Part3	:Red_ChargedBolt01b.part
		Part4	:Red_ChargedBolt02b.part
		Part1	:Red_ChargedBolt06b.part
		Part2	:Red_ChargedBolt05b.part
		Part3	:Red_ChargedBolt01b.part
		Part4	:Red_ChargedBolt02b.part

		Part1	:Red_ChargedBolt06.part
		Part2	:Red_ChargedBolt05.part
		Part3	:Red_ChargedBolt01.part
		Part4	:Red_ChargedBolt02.part
		Part1	:Red_ChargedBolt06.part
		Part2	:Red_ChargedBolt05.part
		Part3	:Red_ChargedBolt01.part
		Part4	:Red_ChargedBolt02.part
		Sound elecboltquick 88 88 .8
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	8

	Event
		Type	start
		At	HandR
		
		Part1	:Red_ChargedBolt06b.part
		Part2	:Red_ChargedBolt05b.part
		Part3	:Red_ChargedBolt01b.part
		Part4	:Red_ChargedBolt02b.part
		Part1	:Red_ChargedBolt06b.part
		Part2	:Red_ChargedBolt05b.part
		Part3	:Red_ChargedBolt01b.part
		Part4	:Red_ChargedBolt02b.part

		Part1	:Red_ChargedBolt06.part
		Part2	:Red_ChargedBolt05.part
		Part3	:Red_ChargedBolt01.part
		Part4	:Red_ChargedBolt02.part
		Part1	:Red_ChargedBolt06.part
		Part2	:Red_ChargedBolt05.part
		Part3	:Red_ChargedBolt01.part
		Part4	:Red_ChargedBolt02.part

		POther  Target
	End

End

Condition
	On 	Time
	Time 	12

	Event
		Type	start
		At	HandR
		
		Part1	:Red_ChargedBolt06b.part
		Part2	:Red_ChargedBolt05b.part
		Part3	:Red_ChargedBolt01b.part
		Part4	:Red_ChargedBolt02b.part
		Part1	:Red_ChargedBolt06b.part
		Part2	:Red_ChargedBolt05b.part
		Part3	:Red_ChargedBolt01b.part
		Part4	:Red_ChargedBolt02b.part

		Part1	:Red_ChargedBolt06.part
		Part2	:Red_ChargedBolt05.part
		Part3	:Red_ChargedBolt01.part
		Part4	:Red_ChargedBolt02.part
		Part1	:Red_ChargedBolt06.part
		Part2	:Red_ChargedBolt05.part
		Part3	:Red_ChargedBolt01.part
		Part4	:Red_ChargedBolt02.part

		POther  Target
	End

End

#Condition
#	On 	Time
#	Time 	15
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	19
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	22
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
##Condition
#	On 	Time
#	Time 	26
#		
#	Event
#
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	30
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther Target	
#	End		
#	
#End
#
#Condition
#	On 	Time
#	Time 	33
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	37
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	40
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	44
#
#	Event
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	47
#
#	Event
#
#		Type	start
#		At	HandR
#		
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#		Part1	:Red_ChargedBolt06b.part
#		Part2	:Red_ChargedBolt05b.part
#		Part3	:Red_ChargedBolt01b.part
#		Part4	:Red_ChargedBolt02b.part
#
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#		Part1	:Red_ChargedBolt06.part
#		Part2	:Red_ChargedBolt05.part
#		Part3	:Red_ChargedBolt01.part
#		Part4	:Red_ChargedBolt02.part
#
#		POther  Target
#	End
#
#End
#
#######################################################################################


#############################################################################
##Hands
############################################################
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Thingy
#		Type	local
#		At	HandR
#		Part1	:Red_ElectricityControl01.part
#		Part2	:Red_ElectricityControl02.part
#		Part3	:Red_ElectricityInnerGlow.part
#		
#		
#
#	End
#
#	Event
#		EName 	ThingyA
#		Type	local
#		At	HandR
#		Part1	:Red_ElectricityOuterGlow.part
#		Part2	:Red_ElectricityArch.part
#		Part3	:Red_ElectricityRing.part
#		
#	End
#
#	
#End
#
#Condition
#	On 	Time
#	Time 	0
#	Event
#		EName 	BurstR
#		Type	local
#		At	HandR
#		Part4	:Red_ElectricitySparkBurst.part
#		Part4	:Red_ElectricitySparkBurst3.part
#		SOund	lightningbolt2 80 30 .76
#		LifeSpan	20
#
#	End
#
#End
#
#Condition
#	On 	PrimeHit
#	
#	Event
#		
#		EName	Prime
#		Type	destroy
#		
#		
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	25  ##55
#	Event
#		
#		EName	Thingy
#		Type	destroy
#		
#		
#	End
#
#	Event
#		
#		EName	ThingyA
#		Type	destroy
#		
#		
#	End
#
#End

End