#########################################################
##	template

FxInfo


#########################################################
#####################  CLOUD 1  #########################
#########################################################


Condition


	Event	
		Type	Local
		At	Chest
		sound	rularuustorm_loop 120 60 .45
	End

End

Condition


	Event	
		ENAME	StormCenter
		Type	Local
		At	Chest
		Sound rumble1_loop 120 120 .3
	End

End

Condition
	On	Cycle
	Time	20
	Event	
		ENAME	Lightning
		Type	Local
		At	StormCenter
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_Flash_01.part
		Sound rumble4 99 99 .33
		sound	silence 99 60 .33
		sound	thunder2 99 60 .29
		Sound lightningbolt4 99 99 .3
		sound	silence 99 60 .29
		#sound	thunder5 99 60 .15
		sound	silence 99 60 .29
		sound	silence 9960 .29
		sound	ss03 99 60 .16
		sound	silence 99 60 .29
		sound	ssthunder 99 60 .3
		sound	silence 99 60 .29
		Lifespan 200
	End			
End

Condition
	On	Cycle
	Time	35
	Event	
		ENAME	Lightning
		Type	Local
		At	StormCenter
		part	:Storm_Lightning_02.part
		part	:Storm_Lightning_Flash_01.part
		Lifespan 200
	End			
End

Condition

	Event	
		ENAME	Lightning
		Type	Local
		At	StormCenter
		part	:Storm_Lightning_03.part
		#part	:Storm_Lightning_Flash_01.part
	End			
End

Condition

	Event	
		ENAME	Cloud
		Type	Local
		At	StormCenter
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part
		part	:Storm_Inward_01.part

	End	
End


#
#Condition
#	On 	Cycle
#	Time 	40	
#
#	Event	
#		ENAME	Lightning
#		Type	Local
#		At	Lightning_Node_Inner
#		POther	Lightning_Node_Outer
#		part	:Storm_Lightning_01.part
#	End			
#End
#
#Condition
#
#	Event	
#		ENAME	Cloud
#		Type	Local
#		At	StormCenter
#		part	:Storm_Heart_01.part
#		part	:Storm_Center_01.part
#		part	:Storm_Center_02.part
#
#	End	
#
#	Event	
#		ENAME	Lightning_Node_Inner
#		Type	Local
#		At	StormCenter
#		Bhvr	:Storm_Lightning_Node_Inner_01.bhvr
#
#	End	
#
#	Event	
#		ENAME	Lightning_Node_Outer
#		Type	Local
#		At	Lightning_Node_Inner
#		Bhvr	:Storm_Lightning_Node_Outer_01.bhvr
#
#	End		
#End
#
#
End		