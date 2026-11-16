#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	root
End

#################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	s1
		Type	Local
		At	root
		Sound tripactivate 15 15 .22
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	s2
		Type	Local
		At	root
		Sound hover_loop 15 15 .15
	End

	Event
		EName	s3
		Type	Local
		At	root
		Sound minebeep_loop 15 15 .13
	End
End

####################################################################################




Condition
	On 	Time
	Time 	0

	Event
		EName	minepivot
		Type	start
		At	root
				
		Geom	gadgetHookup
		#Bhvr	behaviors/minescale.bhvr
	End
	
	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
				
		Geom	Techmine01
##		Bhvr	behaviors/mineFade.bhvr
	End

	Event
		EName	lazer
		Type	start
		At	mine
		altpiv	1
			
		Bhvr	Behaviors/Spin.bhvr		
		Geom	centerdummy
		
	End

	Event
		EName	lightning
		Type	start
		At	lazer
		altpiv	1
		Bhvr	Behaviors/SpinLazer.bhvr		
		Part	:LazerLight.part
		Part	:LazerLightInvert.part
		
				
	End

	Event
		EName	light
		Type	start
		At	mine
				
		Part1	:RedLightPulsingSlowFlat.part
						
	End
	
End

Condition
	On 	Time
	Time 	90
	Event
		EName	lightning1
		Type	start
		At	lazer
		altpiv	1
		Bhvr	Behaviors/SpinLazer.bhvr		
		Part	:LazerLight.part
		Part	:LazerLightInvert.part
		
				
	End
End


Condition
	On 	Time
	Time 	130
	Event
		EName	lightning2
		Type	start
		At	lazer
		altpiv	1
		Bhvr	Behaviors/SpinLazer.bhvr		
		Part	:LazerLight.part
		Part	:LazerLightInvert.part
		
				
	End

End

Condition
	On 	Time
	Time 	220
	Event
		EName	lightning3
		Type	start
		At	lazer
		altpiv	1
		Bhvr	Behaviors/SpinLazer.bhvr		
		Part	:LazerLight.part
		Part	:LazerLightInvert.part
		
				
	End

End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	Mine
		Type	Destroy

	End

	Event		
		Ename	Minepivot
		Type	Destroy

	End

	Event		
		Ename	lazer
		Type	Destroy

	End

	Event		
		Ename	Light
		Type	Destroy

	End

	Event		
		Ename	lightning
		Type	Destroy

	End

	Event		
		Ename	lightning1
		Type	Destroy

	End

	Event		
		Ename	lightning2
		Type	Destroy

	End

	Event		
		Ename	lightning3
		Type	Destroy

	End

	Event		
		Ename	s3
		Type	Destroy

	End

	Event		
		Ename	s2
		Type	Destroy

	End

	Event		
		Ename	s1
		Type	Destroy

	End

End

End