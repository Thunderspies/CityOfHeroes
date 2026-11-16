#########################################################
##	
##

FxInfo


## Notes:  I reduced this fx to the basics - older version has redundant pieces -steve
## Fx are broken down into simple groupings w/ headers -steve





##### Launch Smoke - from Flier when spawned  ##############################################

Condition
	On 	Time
	Time 	0


	Event
		Ename	Smoke_offset_marker
		Type	start
		At 	root
		bhvroverride
			positionoffset	0 25 0
			trackrate .3
			gravity	.015
		end
##		part	:flyer_minecontrail.part
		magnet hair
	End



	Event
		Ename	launchsmoke
		Type	startpositonly
		At 	Smoke_offset_marker
		part	:MineDrone_SmokeRed.part
		part	:MineDrone_SmokeBlack.part
		lifespan 15
		
	End

End


##########  Blades  + puff of smoke when blades pop out   #######################################

Condition
	On 	Time
	Time 	43

	Event
		Ename	Blade_1
		Type	local
		At 	Hair
		geom	drone_Blades01	
		Bhvr	:DroneBladesScale.bhvr
	End

	Event
		Ename	Blade_2
		Type	local
		At 	Hair
		Bhvr	:DroneBladesScale.bhvr
		geom	drone_Blades02

	End



	Event
		Ename	Pop_out_smoke	
		Type	local
		At 	hair
		part	:MineDrone_SmokeBurst.part
		lifespan 10
	End



End


##########  All Red lights    ###################################################################p

Condition
	On 	Time
	Time 	43

	Event
		Ename	Red_lights_Bottom
		Type	local
		At 	hair	
		childFx	:MineDroneLED.fx		
	End
	
	Event
		Ename	Red_lights_Top
		Type	local
		At 	hair	
		childFx	:MineDroneTopLED.fx
			BhvrOverride
			PositionOffset	0 -.02 0	
		End
		
	End


End



End