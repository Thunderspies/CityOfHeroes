#########################################################
##	
##

FxInfo
LifeSpan  90

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
		
		Sound shieldspawner 80 80 .9

	End

End


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
			positionoffset	0 10 0
			spin 0 .15 0
		end
		Flags	AdoptParentEntity
		part	:Flyer_deploySparkTube.part
	
	End
	Event
		Type	start
		At	root 
		bhvroverride
#			positionoffset	0 10 0
			spin 0 .15 0
			pyrrotate	180 0 0
		end
		part	:Flyer_deploySparkTube.part
	
	End
End
Condition
	On Time
	Time 0

	Event
		Type	positonly
		At	uarmL 
		childfx	:flyer_DeploySweeper.fx
		lookat	LarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LarmL 
		childfx	:flyer_DeploySweeper.fx
		lookat	handL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	uarmR 
		childfx	:flyer_DeploySweeper.fx
		lookat	LarmR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LarmR 
		childfx	:flyer_DeploySweeper.fx
		lookat	handR
		bhvroverride
			stretch 1
		end
	End
############## Legs
	Event
		Type	positonly
		At	ulegL 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	LlegL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LlegL 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	footL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ulegR 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	LlegR
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	LlegR 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	footR
		bhvroverride
			stretch 1
		end
	End
###### torso

	Event
		Type	positonly
		At	head 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	hips
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ulegR 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	uarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	positonly
		At	ulegL 
		childfx	:flyer_DeploySweeperLegs.fx
		lookat	uarmR
		bhvroverride
			stretch 1
		end
	End


End

end
