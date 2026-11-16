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
	Inpname	Root
End

LifeSpan	200

########################################################


######################################################################################

Condition

	On 	Time	
	Time	0

	Event	
		ENAME	Overhead
		Type	Posit
		At	Origin
		Geom	OverHeadDummy
		Sound lightningbolt4 120 120 .9
	End

End

Condition
	On 	Time
	Time 	18

	Event
		Type	start
		At	Overhead
		AltPiv	1
		#Part1	:ChargedBolt06.part
		#Part2	:ChargedBolt05.part
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	12

	Event
		Type	start
		At	Overhead
		AltPiv	1
		Part	:SSLSLightningBoltAttackGlow01.part
		POther  Target
		LifeSpan 8
	End

End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	Overhead
		AltPiv	1
		#Part4	:ChargedBolt03.part
		#Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	32

	Event
		Type	start
		At	Overhead
		AltPiv	1
		#Part2	:ChargedBolt01.part
		#Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	Overhead
		AltPiv	1
		#Part4	:ChargedBolt03.part
		#Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	39

	Event
		Type	start
		At	Overhead
		AltPiv	1
		#Part2	:QuickBolt01.part
		#Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	Overhead
		AltPiv	1
		#Part4	:ChargedBolt03.part
		#Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	Overhead
		AltPiv	1
		
		#Part1	:QuickBolt06.part
		#Part2	:QuickBolt05.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	49
		
	Event

		Type	start
		At	Overhead
		AltPiv	1
		
		#Part1	:ChargedBolt01.part
		#Part2	:ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	52
		
	Event

		Type	start
		At	Overhead
		AltPiv	1
		
		#Part1	:QuickBolt03.part
		#Part2	:QuickBolt04.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	56
		
	Event

		Type	start
		At	Overhead
		AltPiv	1
		
		#Part1	:ChargedBolt01.part
		#Part2	:ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	60
		
	Event

		Type	start
		At	Overhead
		AltPiv	1
		
		#Part1	:QuickBolt06.part
		#Part2	:ChargedBolt05.part
		#Part3	:QuickBolt01.part
		#Part4	:ChargedBolt02.part
		
		POther  Target
	End

End

######################################################################################

#############################################################################
##Hands
############################################################


Condition

	On 	Time	
	Time	22

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Root
		Bhvr	Behaviors/SSLSShake.bhvr
		LifeSpan 10
		
		
	End

End


Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime
		Type	destroy
		
		
	End

End

Condition
	On 	Time
	Time 	65


	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End