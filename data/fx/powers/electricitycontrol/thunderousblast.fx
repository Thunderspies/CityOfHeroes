#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

Input  
	Inpname	HandL
End

Input  
	Inpname	T_Root
End

LifeSpan	200

########################################################

Condition
	On 	Time
	Time	0
	
	Event
		Type	Local
		At 	Origin
		Sound Thunderblast2 100 100 1.0

	End
End 


Condition
	On 	Time
	Time 	76

	Event
		EName 	1	#Prime
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	79

	Event
		EName 	Prime
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	85

	Event
		EName 	2
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	92

	Event
		EName 	2.3
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	99

	Event
		EName 	Prime2
		Type	start
		At	HandL
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End

	Event
		EName 	1
		Type	Destroy
		
	End

	Event
		
		Type	Local
		At	Target
		
		Part1	:ShockFieldArchsa.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part
		
		lifespan	30
	End

	Event
		ENAME	H
		Type	start
		At	T_Root
		
		
		geom	roottochestadjustment
		lifespan	30
	End
	
	Event
		EName 	Mallet
		Type	start 
		At	H
		altpiv	1

		Geom	ElectricityHit
		bhvr	behaviors/ScaleBubble2.bhvr
		lifespan	4.0
	End

	Event
		
		Type	start
		At	H
		altpiv	1
		Part1	:ElectricitySparkBurst.part		
		Part2	:ElectricitySparkBurst.part
		Part3	:ElectricitySparkBurst.part
		Part4	:ShockFieldRingA.part
		lifespan	30
	End

	Event
		
		Type	Local
		At	Target
		Part2	:ShockFieldGlow.part
		
		lifespan	5
	End

End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
		
	End

	Event
		EName 	2
		Type	Destroy
		
	End

	Event
		EName 	2.3
		Type	Destroy
		
	End

End

Condition
	On 	Prime3Hit
	
	Event
		EName 	Prime3
		Type	Destroy
		
	End

	Event
		EName 	3
		Type	Destroy
		
	End

End

Condition
	On 	Prime4Hit
	
	Event
		EName 	Prime4
		Type	Destroy
		
	End
	
	Event
		EName 	5
		Type	Destroy
		
	End
	
	Event
		EName 	4
		Type	Destroy
		
	End

End

############################################################################################


########################################################

Condition
	On 	Time
	Time 	74

	Event
		EName 	3
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName 	Prime3
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	83

	Event
		EName 	4
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	93.5

	Event
		EName 	5
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	102

	Event
		EName 	Prime4
		Type	start
		At	HandR
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Bhvr	Behaviors/Fastprojectile6.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

############################################################################################

Condition
	On 	Time
	Time 	75

	Event
		Type	start
		At	HandR
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	78

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	82

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	85

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	89

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	92

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End


Condition
	On 	Time
	Time 	96

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	103

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	106
		
	Event

		Type	start
		At	HandR
		#Part2	:QuickBolt01a.part
		Part3	:QuickBolt06.part
		Part4	:QuickBolt05.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

######################################################################################

Condition
	On 	Time
	Time 	75

	Event
		Type	start
		At	HandL
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	78

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	83

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	86

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End


Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	93

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	96
		
	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt03.part
		Part3	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	103
		
	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	106
		
	Event

		Type	start
		At	HandL
		#Part2	:QuickBolt01a.part
		Part3	:QuickBolt06.part
		Part4	:QuickBolt05.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		
		

	End

	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End
	
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		
	End

	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End
End

#########################################################################
##Spark Bursts
##############################################################

Condition
	On 	Time
	Time 	26
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .55
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	45
	Event
		EName 	BurstR
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .55
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	72
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		#SOund	lightningbolt2 80 30 .6
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End

End

#########################################################################

Condition
	On 	Time
	Time 	110
	Event
		
		EName	Thingy
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End