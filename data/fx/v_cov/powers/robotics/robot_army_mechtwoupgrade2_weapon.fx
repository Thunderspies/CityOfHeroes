#########################################################
##	template

FxInfo


Flags    InheritAlpha
#LifeSpan		300
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		#Sound	Glow3_loop 80 30 .26
	End
End


#########################################################
#################### FX ########################
#########################################################

Condition
	On	Time
	Time	124
	
	Event	
		Type	local
		At	chest
		
		ChildFx		V_COV/Mastermind_pets/Robotics/ProtectorBack.fx
		
		
	
	End	
	
	Event	
		Type	local
		At	chest

		ChildFx		V_COV/Mastermind_pets/Robotics/RobotEyeGLow.fx
		#ChildFx		V_COV/Mastermind_pets/Robotics/RobotEyeGLow_Soldier.fx
		
	
	End

End

End		