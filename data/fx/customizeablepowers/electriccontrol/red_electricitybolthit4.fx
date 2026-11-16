#########################################################
##	EBlast
########################################################
FxInfo

Lifespan  140

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Input  
	Inpname	LArmL
End

Input  
	Inpname	LArmR
End

################################


Condition
	On 	Time
	Time 	2


	Event
		ENAME	start
		Type	start
		At	WepL
		Part1	:ElectricityBolt.part
		part2	:ElectricityBoltInverted.part
		Part3	:ElectricityBolt.part
		part4	:ElectricityBoltInverted.part
		Bhvr	behaviors/Beamprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		
	End



	Event
		ENAME	start2
		Type	start
		At	WepR
		Part1	:ElectricityBolt.part
		part2	:ElectricityBoltInverted.part
		Part3	:ElectricityBolt.part
		part4	:ElectricityBoltInverted.part
		Bhvr	behaviors/Beamprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		
	End

End

#################################################################
##Hands
#################################################################


Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	WepL
		#Part1	powers/ElectricityControl/ElectricityOuterGlow1.part
		Part2	powers/ElectricityControl/ElectricityControl01.part
		Part3	powers/ElectricityControl/ElectricityControl02.part
		Part4	:ElectricitySpark.part
		Part5	powers/ElectricityControl/ElectricityArchSmall.part
		Sound elecfry 60 60 .43

	End

	Event
		EName	HandGlow2
		Type	Local
		At	WepR
		#Part1	powers/ElectricityControl/ElectricityOuterGlow1.part
		Part2	powers/ElectricityControl/ElectricityControl01.part
		Part3	powers/ElectricityControl/ElectricityControl02.part
		Part4	:ElectricitySpark.part
		Part5	powers/ElectricityControl/ElectricityArchSmall.part

	End

	Event
		EName	HandGlow3
		Type	Local
		At	WepL
		Part1	powers/ElectricityControl/ElectricityOuterGlow2.part
	
	End

	Event
		EName	HandGlow4
		Type	Local
		At	WepR
		Part1	powers/ElectricityControl/ElectricityOuterGlow.part
	
	End


	Event
		EName	ArmGlow2
		Type	Local
		At	LArmR
		Part1	powers/ElectricityControl/ElectricityControl01.part
		Part2	powers/ElectricityControl/ElectricityControl02.part
		

		PMagnet	HandGlow2
		LookAt  HandGlow2
	End

	Event
		EName	ArmGlow1
		Type	Local
		At	LArmL
		Part1	powers/ElectricityControl/ElectricityControl01.part
		Part2	powers/ElectricityControl/ElectricityControl02.part
		

		PMagnet	HandGlow1
		LookAt  HandGlow1
	End

End
#################################################################
##BodyCrackles
#################################################################



Condition
	On 	Time
	Time 	30

	Event
		EName 	Start
		Type	Destroy
		
	End

	Event
		EName 	Start2
		Type	Destroy
		
	End


	Event
		EName 	HandGlow3
		Type	Destroy
		
	End

	Event
		EName 	HandGlow4
		Type	Destroy
		
	End

	Event
		EName	HandGlowx1
		Type	Local
		At	WepL
		Part1	powers/ElectricityControl/ElectricityOuterGlow1.part
	
	End

	Event
		EName	HandGlowx2
		Type	Local
		At	WepR
		Part1	powers/ElectricityControl/ElectricityOuterGlow1.part
	
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	HandGlow1
		Type	Destroy
		
	End

	Event
		EName 	HandGlow2
		Type	Destroy
		
	End

	Event
		EName 	HandGlowx1
		Type	Destroy
		
	End

	Event
		EName 	HandGlowx2
		Type	Destroy
		
	End

	Event
		EName 	ArmGlow1
		Type	Destroy
		
	End
	
	Event
		EName 	ArmGlow2
		Type	Destroy
		
	End


End
Condition
	On Time 
	Time 2

	Repeat	8
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\LightningHit.fx

		Lifespan	90
		
	End
		
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End

End

Condition
	On Time 
	Time 0

	Repeat	5
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End
	
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90
		
	End
End

End
