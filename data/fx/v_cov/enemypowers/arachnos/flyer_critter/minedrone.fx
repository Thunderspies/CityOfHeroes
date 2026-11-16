#########################################################
##	
##

FxInfo

#Steve To re-purpose this effect to work with the new spawning anim:
#delete all the events that deal with Prime. Keep launchsmoke and attach it to hair instead of prime. 
#Delete the entire primehit condition and reset change the timed event to coincide with the popping time in the anim.
#The rest of it should work with minimal tweaking. 


##################################
Condition
	On 	Time
	Time 	0


	Event
		Ename	prime
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
		Ename	mesh
		Type	positonly
		At 	prime
##		part	:flyer_minecontrail.part
	End
	Event
		Ename	nair
		Type	local
		At 	hair
	End



	Event
		ename	launchsmoke
		Type	startpositonly
		At 	prime
		part	:MineDrone_SmokeRed.part
		part	:MineDrone_SmokeBlack.part
		lifespan 15
		
	End

End
Condition
	On 	Time
	Time 	43
	Event
		Ename	prime
		Type	destroy
	End	
	Event
		Ename	mesh
		Type	destroy
	End	
	Event
		Ename	prime
		Type	local
		At 	nair
	##	geom	DroneBombCore
##		part	:flyer_minecontrail.part
	End
	Event

		Type	local
		At 	nair
		geom	drone_Blades01
		
		Bhvr	:DroneBladesScale.bhvr
	End

	Event

		Type	local
		At 	nair

		Bhvr	:DroneBladesScale.bhvr
		geom	drone_Blades02

	End

	Event
		
		Type	local
		At 	hair
		
		childFx	:MineDroneLED.fx
		
	End
	
	Event
		
		Type	local
		At 	hair
		
		BhvrOverride
		
			PositionOffset	0 -.02 0
		
		End

		childFx	:MineDroneTopLED.fx
		
	End


	Event
		
		Type	local
		At 	hair

		part	:MineDrone_SmokeBurst.part
		lifespan 10
	End



End
Condition
	On 	primehit	
	Event
		Ename	prime
		Type	destroy
	End	
	Event
		Ename	mesh
		Type	destroy
	End
	Event
		Ename	nair
		Type	destroy
	End
	Event
		Ename	prime2
		Type	local
		At 	hair
	##	geom	DroneBombCore
		part	:flyer_minecontrail.part
	End
	Event

		Type	local
		At 	hair
		geom	drone_Blades01
		
		Bhvr	:DroneBladesScale.bhvr
	End

	Event

		Type	local
		At 	hair

		Bhvr	:DroneBladesScale.bhvr
		geom	drone_Blades02

	End

	Event
		
		Type	local
		At 	hair
		
		childFx	:MineDroneLED.fx
		
	End
	
	Event
		
		Type	local
		At 	hair
		
		BhvrOverride
		
			PositionOffset	0 -.02 0
		
		End

		childFx	:MineDroneTopLED.fx
		
	End

	Event
		
		Type	local
		At 	hair

		part	:MineDrone_SmokeBurst.part
		lifespan 10
	End

End