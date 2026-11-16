#########################################################
##	
##

FxInfo
LifeSpan  1000

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
		ename	curvespot
		Type	local
		At 	origin
#		geom	testing_target
		bhvroverride
			positionoffset	 0 3 3
		end
		
	End

	Event
		ename	curver
		Type	startpositonly
		At 	curvespot
#		geom	testing_target
		magnet target
		bhvr	:flyer_missile_trackshrink.bhvr
	End

	Event
		ename	twister
		Type	local
		At 	curver
#		geom	testing_target
		magnet target
		bhvroverride
			positionoffset	10 0 0
		end
	End

	Event
		ename	prime
		Type	startpositonly
		At 	origin
		magnet	curver
		bhvroverride
			trackrate 2.8
		end
		
	End

	Event
		ename	prime1
		Type	startpositonly
		At 	origin
#		geom	_5th_Missile #silly fatboy
		geom	malta_rocket_grey #minimissile canister
		magnet	twister
		part	:flyer_MissileContrail.part
		part	:flyer_missile_Glow1.part
		part	:flyer_missile_Fire1.part
		part	:flyer_missile_smoke.part
		bhvroverride
			trackrate 2.8
			scale 2 2 2
			Rgb0			60 30 150
		end
		
	End


	Event
		Type	local
		At 	prime1
		part	:flyer_missile_Flash1.part
		part	:flyer_missile_smoke.part
		lifespan 5
		
	End
		

End


Condition
	On 	PrimeHit
	

	Event
		EName	Explosion1
		Type	startpositonly
		At	prime1
		Part	:WhiteCenter.part
		Part	:Flash01.part
		Part	:Smoke1.part
		bhvr	:RocketExploShake2.bhvr
		Lifespan 5		
	End

	Event
		EName 	prime1
		Type	Destroy
	End

End
end
