#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	root
End

LifeSpan	4700

#################################################################################

Condition
	On 	Time
	Time 	0

		
	Event
		Ename	hookup
		Type	local 
		At	root
		Bhvr	:PoisonOffset.bhvr
				
	End

	Event
		EName	minepivot
		Type	start
		At	hookup
		Sound mine2_loop 13 13 .12
		Geom	gadgetHookup
		#Bhvr	behaviors/mineFade.bhvr
	End
	
	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		#Sound	hover_loop 18 16 .14
		
		Bhvr	:GreenGlassMineScale.bhvr
		Geom	PoisonGlassBall
	End
#########################################################################################

	Event
		Ename	Polegear
		Type	start
		At	minepivot
		altpiv	1
		
		Bhvr	:PoleScale.bhvr
		Geom	CloakingPole
	End

#########################################################################################


End

End