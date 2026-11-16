#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha   InheritAnimScale

LifeSpan 0

Input  
	InpName	hips
End


Condition
	On 	Time
	Time 	1

	


	
	Event
		ename	Two
		Type	local 
		At	Root
		bhvr	:Ent_Off.bhvr
#	geom    Testing_Target 

	End

	Event
		ename	One
		Type	start 
		At	Root
		bhvr	:World_Off.bhvr
			lookat	two
		
#	geom    Testing_Target
	End
	Event
		ename	half
		Type	local 
		At	One
		bhvr	:Aim_Half.bhvr


#geom    Testing_Target
	End





	
	Event
		Type	positonly 
		At	Root
		Anim	V_Shaper_Eyelids ##ent type
#		BhvrOverride
#			Scale .1 .1 .1
#		End

			lookat	half
	End
End

End	



			