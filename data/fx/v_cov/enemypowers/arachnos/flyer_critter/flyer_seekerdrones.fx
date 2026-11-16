#########################################################
##	
##

FxInfo
LifeSpan  180

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
		Sound ArachnosSeekerDrone_deploy 180 180 .7
		
	End
End
Condition
	On 	Time
	Time 	0
	
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
		ename	circle
		Type	local
		At 	c_eyes
		part	:flyer_deploy_ray1.part	
		bhvroverride
			scale .1 .1 .1
			positionoffset 0 -10 0
#			pyrrotate 90 0 0
		end
		pmagnet	c_eyes
	End
	Event
		ename	rotor1
		Type	local
		At 	circle
		bhvroverride
			spin 0 .02 0
			spinjitter 0 .01 0
		end
		
		pmagnet	waist
	End
	Event
		ename	circle1
		Type	local
		At 	rotor1
		part	:flyer_deploy_ray1.part
		bhvr	:flyer_deploy_rayscale.bhvr		
		pmagnet	waist
	End
	Event
		ename	rotor2
		Type	local
		At 	circle
		bhvroverride
			spin 0 -.02 0
			spinjitter 0 .01 0
		end
		
		pmagnet	waist
	End
	Event
		ename	circle2
		Type	local
		At 	rotor2
		part	:flyer_deploy_ray1.part
		bhvr	:flyer_deploy_rayscale.bhvr		
		pmagnet	waist
	End

End

end
