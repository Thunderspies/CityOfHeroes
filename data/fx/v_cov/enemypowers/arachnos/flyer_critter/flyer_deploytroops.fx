#########################################################
##	
##

FxInfo
LifeSpan  300

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin	
		Sound ArachnosFlyerBeam 180 180 .6
		
	End
End


Condition
	On 	time
	Time 	0
	
#	Event
#		ename	harpoon
#		Type	positonly
#		At 	waist
#		bhvroverride
#			stretch 1
#		end
#		childfx	:flyer_deploy_harpoon.fx
#		lookat	target
#	End

	Event
		ename	droptest1
		Type	startpositonly
		At 	origin
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		bhvroverride
			initialvelocity	.01 -5 .01
		end
#		CEvent 	V_COV\EnemyPowers\Arachnos\Flyer_Critter\flyer_phystester.fx #V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#		CDestroy 0
		lifespan	30
	End

	Event
		ename	prime
		Type	start
		At 	hips
		magnet	droptest1
#		childFX	:flyer_deploy_dart.fx
		bhvroverride
			trackrate 4.9
			positionoffset 0 15 0
		end
	End
	Event
		ename	logo
		Type	local
		At 	hips
		part	:flyer_deploy_logo.part
		part	:flyer_deploy_logo.part
		bhvroverride
			positionoffset 0 -1.1 0
		end
	End
	
	Event
		ename	poot
		Type	positonly
		At 	root
#		geom testing_target
	End

	Event
		ename	prime1
		Type	local
		At 	poot
		
#		geom	testing_target
		bhvroverride
			positionoffset 0 -60 0
			scale 10 10 10
		end
	End


end



Condition
	On 	primehit


	Event
		EName 	prime1
		Type	Destroy
	End

	Event
		ename	prime1
		Type	startpositonly
		At 	hips
		magnet	droptest1
#		geom	testing_target
#		childFX	:flyer_deploy_dart.fx
		bhvroverride
			trackrate 10
		end
		
		pmagnet	origin
	End


end
Condition
	On 	time
	Time 	20
###########################
	Event
		ename	drop1L
		Type	local
		At 	hips
#		geom	testing_target
		bhvroverride
			positionoffset	1.5 0 3.5
		end
	End
	Event
		ename	poon1L
		Type	start
		At 	drop1L
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		lifespan	25
	End
	Event
		ename	prime2
		Type	start
		At 	drop1L
#		geom	testing_target
		bhvroverride
			positionoffset	0 15 0
			trackrate 5
		end
		magnet	poon1L
	End

###########################
	Event
		ename	drop1R
		Type	local
		At 	hips
#		geom	testing_target
		bhvroverride
			positionoffset	-1.5 0 3.5
		end
	End

	Event
		ename	poon1R
		Type	start
		At 	drop1R
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		lifespan	25
	End
	Event
		ename	prime3
		Type	start
		At 	drop1R
#		geom	testing_target
		bhvroverride
			positionoffset	0 15 0
			trackrate 5
		end
		magnet	poon1R
	End
end


Condition
	On 	prime2hit

	Event
		ename poon1L
		type destroy
	end
	Event
		ename launch1L
		type local
		at	hips
		bhvroverride
			positionoffset	.3 -.6 2.2
		end

	end
	Event
		ename	harpoon1L
		Type	positonly
		At 	launch1L
		bhvroverride
			stretch 1
		end
		childfx	:flyer_deploy_harpoon.fx
		lookat	prime2
	End
end

Condition
	On 	prime3hit

	Event
		ename poon1R
		type destroy
	end
	Event
		ename launch1R
		type local
		at	hips
		bhvroverride
			positionoffset	-.3 -.6 2.2
		end

	end
	Event
		ename	harpoon1R
		Type	positonly
		At 	launch1R
		bhvroverride
			stretch 1
		end
		childfx	:flyer_deploy_harpoon.fx
		lookat	prime3
	End
end


Condition
	On 	time
	Time 	25
###########################
	Event
		ename	drop2L
		Type	local
		At 	hips
#		geom	testing_target
		bhvroverride
			positionoffset	3.5 0 1
		end
	End
	Event
		ename	poon2L
		Type	start
		At 	drop2L
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		lifespan	25
	End
	Event
		ename	prime4
		Type	start
		At 	drop2L
#		geom	testing_target
		bhvroverride
			positionoffset	0 15 0
			trackrate 5
		end
		magnet	poon2L
	End
###########################
	Event
		ename	drop2R
		Type	local
		At 	hips
#		geom	testing_target
		bhvroverride
			positionoffset	-3.5 0 1
		end
	End

	Event
		ename	poon2R
		Type	start
		At 	drop2R
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		lifespan	25
	End
	Event
		ename	prime5
		Type	start
		At 	drop2R
#		geom	testing_target
		bhvroverride
			positionoffset	0 15 0
			trackrate 5
		end
		magnet	poon2R
	End
end


Condition
	On 	prime4hit

	Event
		ename poon2L
		type destroy
	end
	Event
		ename launch2L
		type local
		at	hips
		bhvroverride
			positionoffset	1.55 -.9 1
		end

	end
	Event
		ename	harpoon2L
		Type	positonly
		At 	launch2L
		bhvroverride
			stretch 1
		end
		childfx	:flyer_deploy_harpoon.fx
		lookat	prime4
	End
end
Condition
	On 	prime5hit

	Event
		ename poon2R
		type destroy
	end
	Event
		ename launch2R
		type local
		at	hips
		bhvroverride
			positionoffset	-1.55 -.9 1
		end

	end
	Event
		ename	harpoon2R
		Type	positonly
		At 	launch2R
		bhvroverride
			stretch 1
		end
		childfx	:flyer_deploy_harpoon.fx
		lookat	prime5
	End
end



Condition
	On 	time
	Time 	30
###########################
	Event
		ename	drop3L
		Type	local
		At 	hips
#		geom	testing_target
		bhvroverride
			positionoffset	2.5 0 -3
		end
	End
	Event
		ename	poon3L
		Type	start
		At 	drop3L
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		lifespan	25
	End
	Event
		ename	prime6
		Type	start
		At 	drop3L
#		geom	testing_target
		bhvroverride
			positionoffset	0 15 0
			trackrate 5
		end
		magnet	poon3L
	End
###########################
	Event
		ename	drop3R
		Type	local
		At 	hips
#		geom	testing_target
		bhvroverride
			positionoffset	-2.5 0 -3
		end
	End

	Event
		ename	poon3R
		Type	start
		At 	drop3R
#		geom	testing_target
		bhvr	:flyer_deploy_dart.bhvr
		lifespan	25
	End
	Event
		ename	prime7
		Type	start
		At 	drop3R
#		geom	testing_target
		bhvroverride
			positionoffset	0 15 0
			trackrate 5
		end
		magnet	poon3R
	End
###########################
End



Condition
	On 	prime6hit

	Event
		ename poon3L
		type destroy
	end
	Event
		ename launch3L
		type local
		at	back
#		geom	testing_target
		bhvroverride
			positionoffset	0.5 0 0
		end

	end
	Event
		ename	harpoon3L
		Type	positonly
		At 	launch3L
		bhvroverride
			stretch 1
		end
		childfx	:flyer_deploy_harpoon.fx
		lookat	prime6
	End
end

Condition
	On 	prime7hit

	Event
		ename poon3R
		type destroy
	end
	Event
		ename launch3R
		type local
		at	back
#		geom	testing_target
		bhvroverride
			positionoffset	-0.5 0 0
		end

	end
	Event
		ename	harpoon3R
		Type	positonly
		At 	launch3R
		bhvroverride
			stretch 1
		end
		childfx	:flyer_deploy_harpoon.fx
		lookat	prime7
	End
end






Condition
	On 	Time
	Time 	20

	Event
		ename	hippy
		Type	positonly
		At 	hips
	End

	Event
		ename	circle
		Type	positonly
		At 	prime1
		part	:flyer_deploy_ray1.part	
		bhvroverride
			scale 1 1 1
		end
		pmagnet	origin
	End
	Event
		ename	rotor1
		Type	positonly
		At 	prime1
		bhvroverride
			spin 0 .02 0
			spinjitter 0 .01 0
		end
		
		pmagnet	origin
	End
	Event
		ename	circle1
		Type	local
		At 	rotor1
		part	:flyer_deploy_ray1.part
		bhvr	:flyer_deploy_rayscale.bhvr		
		pmagnet	origin
	End
	Event
		ename	rotor2
		Type	positonly
		At 	prime1
		bhvroverride
			spin 0 -.02 0
			spinjitter 0 .01 0
		end
		
		pmagnet	origin
	End
	Event
		ename	circle2
		Type	local
		At 	rotor2
		part	:flyer_deploy_ray1.part
		bhvr	:flyer_deploy_rayscale.bhvr		
		pmagnet	origin
	End


End

Condition
	On 	Time
	Time 	240

	event
		ename	logo
		type	destroy
	end
	event
		ename	circle
		type	destroy
	end
	event
		ename	circle1
		type	destroy
	end
	event
		ename	circle2
		type	destroy
	end

end
end
