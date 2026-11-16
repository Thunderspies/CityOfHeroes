#########################################################
##	
##

FxInfo
#LifeSpan  90

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
	On Time
	Time 60

	Event
		Type	local
		At	root 
		childfx	:bot_activate_cables.fx
		
	End
end
Condition
	On Time
	Time 0
	Event
		EName 	Prime
		Type	start
		At	root 
		Bhvr	behaviors/fadein.bhvr
		bhvroverride
			FadeInLength		180
			positionoffset	0 12 0
			spin 0 .15 0
			scale	2.3	1.2	2.3
		end
		Flags	AdoptParentEntity
		part	:bot_activate_SparkTube.part
		part	:bot_activate_SparkTube.part
##		lifespan	200
	
	End
	Event
		Type	start
		At	root 
		bhvroverride
			spin 0 .15 0
			pyrrotate	180 0 0
			scale	2.3	1.2	2.3
		end
		part	:bot_activate_SparkTube.part
		part	:bot_activate_SparkTube.part
		lifespan	200
	
	End
	Event
		Type	local
		At	root 
		childfx	:bot_activate_TPLeft.fx
		lifespan	200
	End
	Event
		Type	local
		At	root 
		childfx	:bot_activate_TPRight.fx
		lifespan	200
	
	End





	Event	
		ename	buttspin
		Type	local
		At	waist 
		bhvroverride
			spin .3 0 0
			spinjitter .1 0 0
		end
	End	
	Event	
		
		Type	Local
		At	buttspin 
		bhvroverride
			positionoffset 2 1.2 0
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin
		bhvroverride
			positionoffset 2 -1.2 0
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		
		Type	Local
		At	buttspin 
		bhvroverride
			positionoffset -.8 0 1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin
		bhvroverride
			positionoffset -.8 0 -1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		
		Type	Local
		At	buttspin
		bhvroverride
			positionoffset .8 0 1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin
		bhvroverride
			positionoffset .8 0 -1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End

##########
	Event	
		ename	buttspin2
		Type	local
		At	waist 
		bhvroverride
			spin .3 0 0
			spinjitter .1 0 0
		end
	End	
	Event	
		
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset 2 1.2 0
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset 2 -1.2 0
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset -2 1.2 0
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset -2 -1.2 0
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		
		Type	Local
		At	buttspin2 
		bhvroverride
			positionoffset -.8 0 1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset -.8 0 -1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset .8 0 1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End
	Event	
		Type	Local
		At	buttspin2
		bhvroverride
			positionoffset .8 0 -1.7
		end
		part	:bot_activate_SparkLegs.part
		part	:bot_activate_SparkLegs.part

	End

End
