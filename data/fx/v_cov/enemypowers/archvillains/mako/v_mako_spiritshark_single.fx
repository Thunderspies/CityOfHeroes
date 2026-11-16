#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha  # InheritAnimScale

lifespan 150

#		Anim	Shark_Spirit_Charge ##ent type

#################### very complicated ###################
## 
## 
## 
## 
## 
## 
## 

Condition
	On 	Time
	Time 	0


	Event
		ename	pointer
		Type	Start
		At	origin
		#geom	testing_target
		#Lookat	Target
	End
	
	
	Event
		ename	hitarg
		Type	Local
		At	pointer
		#geom	testing_target
		Bhvroverride
			PositionOffset 0 65 100
			startjitter 0 30 0
#			initialvelocity 0 0 .1
		end
	End

	Event
		ename	histart
		Type	Local
		At	pointer
		#geom	testing_target
		Bhvroverride
			PositionOffset 0 0 100
		end
	End	
	
	Event
		ename	prime
		Type	start 
		At	histart
		#geom	testing_target
		Bhvroverride
#			PositionOffset 0 0 100
			trackrate 1
		end
		magnet	hitarg
		lookat	origin
	End

	Event
		ename	spin1
		Type	positonly 
		At	prime
		#geom	testing_target
		Bhvroverride
#			PositionOffset 0 0 50
#			initialvelocityjitter 0 0 .1
#			initialvelocity 0 0 .1
#			trackrate 1
		end
		bhvr	:V_Mako_Wandershrink.bhvr
#		magnet	hitarg
	End



###############

	Event
		ename	spot1
		Type	local 
		At	spin1
		#geom	testing_target
		Bhvroverride
			PositionOffset 0 0 20
#			initialvelocity 0 1 0
		end
	End
	
	Event
		ename	counter
		Type	local 
		At	spot1
		#geom	testing_target
		Bhvroverride
			initialvelocity 0 -.98 0
		end
	End


#########################
	
	Event
		ename	shark
		Type	start 
		At	origin
		#geom	testing_target
		Anim	Shark_Spirit_Charge ##ent type
		Bhvroverride
			trackrate .8
			scale	.3 .3 .3
		end
		magnet	counter
	End

	Event
		EName 	nosetip
		Type	Local 
		At	shark
		AnimPiv head
		Bhvroverride
			PositionOffset 0 0 3
			scale	3 3 3
		end
		childFX Volumes\Water\Ripple\ripple.fx
	End



End


Condition
On 	PrimeHit 
	Event
		ename	spot1
		Type	startpositonly 
		At	nosetip
		Bhvroverride
			trackrate .8
			scale	.5 .5 .5
		end
		lifespan 10
	End	
	Event
		ename	prime1
		Type	startpositonly 
		At	root
		Bhvroverride
			trackrate .4
			scale	.5 .5 .5
		end
		magnet head
	End

	Event
		Type	startpositonly 
		At	nosetip
		Bhvroverride
			scale	2 1 2
		end
		childFX :V_Mako_Spiritshark_splash.fx
	End
end

Condition
On 	Prime1Hit 


	Event
		ename	shark
		Type	destroy 
	End
end

End	



			