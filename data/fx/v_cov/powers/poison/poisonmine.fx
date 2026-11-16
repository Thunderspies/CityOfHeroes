#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	root
End

##LifeSpan	470

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	minepivot
		Type	start
		At	root
		Sound poisonmine_loop 18 18 .33
		Geom	gadgetHookup
		#Bhvr	behaviors/mineFade.bhvr
	End
	
	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		Sound air_loop 18 18 .22
		
		Bhvr	:GreenGlassMineScale.bhvr
		Geom	PoisonGlassBall
	End
#########################################################################################

#	Event
#		EName 	minesplat
#		Type	local
#		At	Root
#		Geom	CenterDummy
#		BHVR	:SplatRotate.bhvr
#	End
End

Condition
	On 	Cycle
	Time 	30

	Event
		EName	hk
		Type	start
		At	minepivot
		altpiv	1
		
		Bhvr	:splatElipse.bhvr
		splat	Generic_Ellipse.tga
		Lifespan	10
	End
End	

Condition
	On 	Cycle
	Time 	13
	Chance	.2

	Event
		EName	hk
		Type	start
		At	minepivot
		altpiv	1
		
		Bhvr	:splatElipse.bhvr
		splat	Generic_Ellipse.tga
		Lifespan	10
	End
End

Condition
	On 	Cycle
	Time 	13
	Chance	.62

	Event
		EName	hk
		Type	start
		At	minepivot
		altpiv	1
		
		Bhvr	:splat.bhvr
		splat	Generic_Ring.tga
		Lifespan	10
	End
End

End