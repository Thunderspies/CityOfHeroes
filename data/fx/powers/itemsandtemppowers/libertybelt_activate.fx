#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

lifespan	60
########################################################
Condition
	On 	Time
	Time 	15


	Event
		Type	Local 
		At	hips
		Sound Liberty_belt3 120 120 .99
	End

End




Condition
	On 	Time
	Time 	15


	Event
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/Fade.bhvr

		Part1	:Ms_Liberty_Belt.part
		Part2	:Ms_Liberty_ArcsLarge01.part
		Part3	:Ms_Liberty_Tendrils.part
#		Part4	POWERS\Ability\AbilitySparklingDots.part
		
	End

End

End
##################################

