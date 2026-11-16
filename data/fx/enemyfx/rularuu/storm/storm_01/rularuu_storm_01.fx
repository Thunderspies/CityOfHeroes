#########################################################
##	template

FxInfo


#########################################################
#####################  CLOUD 1  #########################
#########################################################


Condition


	Event	
		ENAME	StormCenter
		Type	Local
		At	Chest
		sound	tornado_loop 80 60 .18
	End

End

Condition


	Event	
		Type	Local
		At	Chest
		sound	rumble1_loop 80 60 .27
	End

End

Condition
	On	Cycle
	Time	80
	Event	
		ENAME	Lightning
		Type	Local
		At	StormCenter
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_Flash_01.part
		Sound lightningbolt4 70 70 .28
		Sound rumble4 60 60 .25
		sound	thunder4 60 60 .18
		sound	silence 60 60 .34
		sound	silence 60 60 .34
		sound	thunder2 60 60 .20
		sound	thunder5 60 60 .17
		sound	silence 60 60 .33
		sound	ssthunder 60 60 .22
		sound	silence 60 60 .3
		Lifespan 200
	End			
End

Condition
	On	Cycle
	Time	100
	Event	
		ENAME	Lightning
		Type	Local
		At	StormCenter
		part	:Storm_Lightning_02.part
		part	:Storm_Lightning_Flash_01.part
		#Sound	rumble4 80 60 .5
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
		#Sound	lightningbolt4 80 60 .6
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