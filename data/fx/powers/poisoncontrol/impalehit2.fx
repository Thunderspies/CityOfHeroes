#########################################################
##	
##

FxInfo

Input  
	InpName	chest
End

##################################
Condition
	On 	Time
	Time 	0

	Event	
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		ChildFX Genericcontinuing/DeBuffDamageShard.fx
		Lifespan 200
		
	End

	Event

		
		Type	local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	quill01
		#Lookat	Target
		
	End

	Event

		
		Type	local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	quill03
		#Lookat	Target
		
	End

	Event

		
		Type	local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	quill07
		#Lookat	Target
		
	End

	

	Event

		
		Type	local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	quill06
		#Lookat	Target
	
	End

	
End
	

End				