#########################################################
##	
##

FxInfo
LifeSpan  100

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
		ename	endzor
		Type	Local
		At 	origin
#		geom	testing_target
		lifespan 80
		bhvroverride
			positionoffset	0 0 1.1
		end
	End
 
	Event
		ename roffset
		Type	positonly
		At 	origin
		lifespan 20
		lookat endzor
	End

	Event
		Type	local
		At 	roffset
		part	:flyer_muzzleglow.part
		part	:flyer_muzzleglow_big.part
		lifespan 15
		bhvroverride
			pyrrotate	90 0 0
		end
	End
End


Condition
	On 	time
	Time 	10
	

	Event
		ename prime1
		Type	start
		At 	origin
		part	:flyer_burstshotStreak.part
		part	:flyer_burstshot.part
		bhvroverride
			positionoffset 0 .1 0
			scale	1 1 1
			trackrate 4
		end
		magnet	endzor
		lifespan 80
	End
	Event
                Type 	SetLight
                LifeSpan 	5
		bhvroverride
			PulsePeakTime		0
			PulseBrightness		5
		end
        End
End



Condition
	On 	prime1hit
	
	Event
		EName 	all
		Type	Destroy
		

	End
end


end