#########################################################
##	BaseTRT Tech Turret Sapper Power
########################################################
FxInfo

LifeSpan	200

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Emit_L
		Type	Local	
		At	Origin
		bhvroverride
			PositionOffset		0 6.7 0
		end

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
#		Geom	Testing_TargetB
#		Sound	CABuildup 60.0 30.0 .4
	End

	Event	
		EName	Emit_R
		Type	Local	
		At	Origin
		bhvroverride
			PositionOffset		0 6.7 0
		end

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_BarrelLeft.bhvr
#		Geom	Testing_TargetB
	End
End

######################################################################################################
##########################################  Barrel Flare  ############################################
######################################################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	Emit_R
		Part1	POWERS\ElectricityControl\ChargedBolt06.part
		Part2	POWERS\ElectricityControl\ChargedBolt05.part
		Part3	POWERS\ElectricityControl\ChargedBolt01.part
		Part4	POWERS\ElectricityControl\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	Emit_R
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	32

	Event
		Type	start
		At	Emit_R
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	Emit_R
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	39

	Event
		Type	start
		At	Emit_R
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	Emit_R
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	Emit_R
		#Part2	POWERS\ElectricityControl\QuickBolt01a.part
		Part3	POWERS\ElectricityControl\ChargedBolt06.part
		Part4	POWERS\ElectricityControl\ChargedBolt05.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

######################################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	Emit_L
		Part1	POWERS\ElectricityControl\ChargedBolt06.part
		Part2	POWERS\ElectricityControl\ChargedBolt05.part
		Part3	POWERS\ElectricityControl\ChargedBolt01.part
		Part4	POWERS\ElectricityControl\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	Emit_L
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	Emit_L
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	36

	Event
		Type	start
		At	Emit_L
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	Emit_L
		Part2	POWERS\ElectricityControl\ChargedBolt01.part
		Part3	POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	43

	Event
		Type	start
		At	Emit_L
		Part4	POWERS\ElectricityControl\ChargedBolt03.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	Emit_L
		#Part2	POWERS\ElectricityControl\QuickBolt01a.part
		Part3	POWERS\ElectricityControl\ChargedBolt06.part
		Part4	POWERS\ElectricityControl\ChargedBolt05.part
		Part5	POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	Emit_R
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		SOund	lightningbolt3 80 30 .76
		

	End

	Event
		EName 	Thingy1
		Type	local
		At	Emit_L
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		
	End


	Event
		EName 	ThingyA
		Type	local
		At	Emit_R
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		
	End

	Event
		EName 	ThingyB
		Type	local
		At	Emit_L
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
		At	Emit_R
		Part4	POWERS\ElectricityControl\ElectricitySparkBurst.part
		
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	Emit_L
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