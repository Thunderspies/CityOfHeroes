#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Flags    InheritAlpha

########################################################
## hookup
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	chest
		Geom	proplink
			
	End

	Event
		EName 	link1
		Type	Local 
		At	Back
		## Altpiv	1
		# Sound heli_loop 50 50 .07
		Part1	:Jetpack_turbine_Smoke.part
		Bhvr	enemyfx/v_goldbrickers/Jetpack_Smoke.bhvr
#		Geom	Testing_TargetB
	End

	Event
		EName 	link4
		Type	Local 
		At	Prime
		 AltPiv 1
		Bhvr	enemyfx/v_goldbrickers/Jetpack_Propeller_spin.bhvr
		Geom	Propeller2
		
	End

	Event
		EName	JetRight
		Type	Local
		At	Back

		Childfx	ENEMYFX\V_Goldbrickers\GB_Thruster.fx
#		Geom	Testing_TargetB
	End
End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	link1
		Type	Destroy
	
	End

	Event
		EName 	link2
		Type	Destroy
	
	End

##	Event   ###(note: they are not supposed to have a propeller -steve)
##		EName 	link3
##		Type	Local 
##		At	Prime
##		 AltPiv 1
##		
##		Geom	Propeller
##
##	End

End

End
##################################

