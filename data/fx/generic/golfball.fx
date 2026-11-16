#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

Input  
	InpName	WepL
End

Input  
	InpName	Root
End

Input  
	InpName	head
End

##Lifespan	200

##################################
Condition
	On 	Time
	Time 	0


	Event
		EName	Ball_Spawn
		Type	Start
		At	Mystic
		Anim	Prop_golfball
			
	End

	Event
		EName	Ball
		Type	Local
		At	Ball_Spawn
		AnimPiv Waist
		Geom	Prop_golfball
		Bhvr	Behaviors/GenericParticleFade.bhvr
	
	End

End	


Condition
	On 	Time
	Time 	160

	Event
		EName	all
		Type	Destroy
	
	End

End	

End