#########################################################
##	Frost Rage Revive
########################################################

FxInfo

Lifespan	90

######  rise up    ##################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Chest
		Sound Intel_loop 30 30 .45
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 50
	End

End




Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Chest
		Part3	POWERS\Ability\AbilityDots.part
		Part4	POWERS\Ability\AbilitySparklingDots.part
		Part5	POWERS\Ability\White_ContinuingRays.part
		Lifespan 50
	End

End

#######  infuse life  ##########################


Condition
	On 	Time
	Time 	10

	Event
		EName   Wind
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWind.bhvr
		geom	FX_TmpCtrlWisp01
		part1	POWERS\ColdControl\IceMistUpwards.part
		part2	POWERS\ColdControl\SnowMistUpwards.part
		
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName   WindFast
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFast.bhvr
		geom	FX_TmpCtrlWisp01
	End
End

Condition
	On 	Time
	Time 	20
	Event
		EName   WindFirst
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFirst.bhvr
		geom	FX_TmpCtrlWisp01
		
	End
End

Condition
	On	Time
	Time    36
	Event	
		EName 	Wind
		Type	Destroy
	End
End

Condition
	On	Time
	Time    39
	Event	
		EName 	WindFast
		Type	Destroy
	End
End

Condition
	On	Time
	Time   44
	Event	
		EName 	WindFirst
		Type	Destroy
	End

End



End
##################################

