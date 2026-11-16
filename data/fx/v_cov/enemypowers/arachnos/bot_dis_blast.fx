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

LifeSpan	80

########################################################

Condition
	On 	time
	Time 	20
	
	Event
		ename	Charger
		Type	local
		At	waist
		lifespan 30
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
	Time 	35

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
	Time 	38

	Event
		Type	start
		At	C_HandR
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	C_HandR
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	45

	Event
		Type	start
		At	C_HandR
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

#Condition
#	On 	Time
#	Time 	49
#
#	Event
#		Type	start
#		At	C_HandR
#		Part2	POWERS\ElectricityControl\ChargedBolt01.part
#		Part3	POWERS\ElectricityControl\ChargedBolt02.part
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	52
#
#	Event
#		Type	start
#		At	C_HandR
#		Part4	POWERS\ElectricityControl\ChargedBolt03.part
#		Part5	POWERS\ElectricityControl\ChargedBolt04.part
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	56
#		
#	Event
#
#		Type	start
#		At	C_HandR
#		#Part2	POWERS\ElectricityControl\QuickBolt01a.part
#		Part3	POWERS\ElectricityControl\ChargedBolt06.part
#		Part4	POWERS\ElectricityControl\ChargedBolt05.part
#		Part5	POWERS\ElectricityControl\ChargedBolt04.part
#		POther  Target
#	End
#
#End

######################################################################################

Condition
	On 	Time
	Time 	35

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
	Time 	38

	Event
		Type	start
		At	C_HandL
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	C_HandL
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	45

	Event
		Type	start
		At	C_HandL
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

#Condition
#	On 	Time
#	Time 	49
#
#	Event
#		Type	start
#		At	C_HandL
#		Part2	POWERS\ElectricityControl\ChargedBolt01.part
#		Part3	POWERS\ElectricityControl\ChargedBolt02.part
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	52
#
#	Event
#		Type	start
#		At	C_HandL
#		Part4	POWERS\ElectricityControl\ChargedBolt03.part
#		Part5	POWERS\ElectricityControl\ChargedBolt04.part
#		POther  Target
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	56
#		
#	Event
#
#		Type	start
#		At	C_HandL
#		#Part2	POWERS\ElectricityControl\QuickBolt01a.part
#		Part3	POWERS\ElectricityControl\ChargedBolt06.part
#		Part4	POWERS\ElectricityControl\ChargedBolt05.part
#		Part5	POWERS\ElectricityControl\ChargedBolt04.part
#		POther  Target
#	End
#
#End

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Thingy
		Type	local
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		SOund	lightningbolt3 80 30 .76
		

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
	Time 	40
	Event
		EName 	BurstR
		Type	local
		At	C_HandR
		Part4	POWERS\ElectricityControl\ElectricitySparkBurst.part
		
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
	Time 	55
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