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
	Inpname	C_HandR
End

Input  
	Inpname	C_HandL
End

LifeSpan	250

########################################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	chest
		Sound elec_teslabot 88 88 .89
	End
End


Condition
	On 	time
	Time 	20
	
	Event
		ename	Charger
		Type	local
		At	waist
		lifespan 60
	end
End

Condition
	On 	cycle
	Time 	2
	
	Event
		Type	local
		At	Charger
		childfx :Bot_Dis_Arcs.fx
		lifespan 20
	end
End


Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ChargedBolt06.part
		Part2	POWERS\ElectricityControl\ChargedBolt05.part
		Part3	POWERS\ElectricityControl\ChargedBolt01.part
		Part4	POWERS\ElectricityControl\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	C_HandR
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_UarmR
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	C_HandR
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  T_LLegL
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	C_HandR
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_HandR
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	C_HandR
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  TLarmL
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	C_HandR
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_UlegL
	End

End

Condition
	On 	Time
	Time 	51
		
	Event

		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  T_Head
	End

End

Condition
	On 	Time
	Time 	54
		
	Event

		Type	start
		At	C_HandR
		Part3	POWERS\ElectricityControl\ChargedBolt03.part
		Part4	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_Hips
	End

End

Condition
	On 	Time
	Time 	57
	
	Event

		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  T_UarmL
	End	

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	C_HandR
		Part3	POWERS\ElectricityControl\ChargedBolt03.part
		Part4	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_FootR
	End

End

Condition
	On 	Time
	Time 	64

	Event

		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  T_HandR
	End

End

Condition
	On 	Time
	Time 	67

	Event

		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt06.part
		Part3	POWERS\ElectricityControl\ChargedBolt05.part
		Part4	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

######################################################################################

Condition
	On 	Time
	Time 	31

	Event
		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ChargedBolt06.part
		Part2	POWERS\ElectricityControl\ChargedBolt05.part
		Part3	POWERS\ElectricityControl\ChargedBolt01.part
		Part4	POWERS\ElectricityControl\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	34

	Event
		Type	start
		At	C_HandL
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_LlegR
	End

End

Condition
	On 	Time
	Time 	38

	Event
		Type	start
		At	C_HandL
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  T_LarmR
	End

End

Condition
	On 	Time
	Time 	41

	Event
		Type	start
		At	C_HandL
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_UarmL
	End

End

Condition
	On 	Time
	Time 	45

	Event
		Type	start
		At	C_HandL
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  T_Head
	End

End

Condition
	On 	Time
	Time 	48

	Event
		Type	start
		At	C_HandL
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_FootR
	End

End

Condition
	On 	Time
	Time 	52
		
	Event

		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  T_HandR
	End

End

Condition
	On 	Time
	Time 	55
		
	Event

		Type	start
		At	C_HandL
		Part3	POWERS\ElectricityControl\ChargedBolt03.part
		Part4	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_LarmL
	End

End

Condition
	On 	Time
	Time 	58
	
	Event

		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  T_chest
	End	

End

Condition
	On 	Time
	Time 	62

	Event

		Type	start
		At	C_HandL
		Part3	POWERS\ElectricityControl\ChargedBolt03.part
		Part4	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_FootL
	End

End

Condition
	On 	Time
	Time 	65

	Event

		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  T_UlegR
	End

End

Condition
	On 	Time
	Time 	68

	Event

		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ChargedBolt01.part
		Part2	POWERS\ElectricityControl\ChargedBolt06.part
		Part3	POWERS\ElectricityControl\ChargedBolt05.part
		Part4	POWERS\ElectricityControl\ChargedBolt04.part
		POther  T_Hips
	End

End


#############################################################################
##C_Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		#Sound	cagebolt2 80 30 .8
		
		

	End

	Event
		EName 	Thingy1
		Type	local
		At	C_HandL
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		
	End


	Event
		EName 	ThingyA
		Type	local
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		
	End

	Event
		EName 	ThingyB
		Type	local
		At	C_HandL
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		
	End
End

Condition
	On 	Time
	Time 	25
	Event
		EName 	BurstR
		Type	local
		At	C_HandR
		Part4	POWERS\ElectricityControl\ElectricitySparkBurst.part
		#SOund	lightningbolt2 80 30 .76
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	C_HandL
		Part4	POWERS\ElectricityControl\ElectricitySparkBurst.part
		LifeSpan	20

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
	Time 	71
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