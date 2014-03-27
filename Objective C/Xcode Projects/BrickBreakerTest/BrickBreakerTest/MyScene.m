//
//  MyScene.m
//  BrickBreakerTest
//
//  Created by Philippe Belley on 2014-03-09.
//  Copyright (c) 2014 Philippe Belley. All rights reserved.
//

#import "MyScene.h"

#define BRICK_RECT(width, height) CGRectMake(-width/2, -height/2, width, height)
#define STD_BRICK CGSizeMake(50, 30)
#define MAX_BRICKS 7

static const uint32_t ballCategory     =  0x1 << 0;
static const uint32_t brickCategory    =  0x1 << 1;
static const uint32_t bordersCategory  =  0x1 << 2;

@interface MyScene ()
{
    SKEmitterNode* _emitter;
}
@end

@implementation MyScene

-(id)initWithSize:(CGSize)size {    
    if (self = [super initWithSize:size]) {
        /* Setup your scene here */
        [self commonInit];
    }
    return self;
}

-(void)commonInit
{
    self.backgroundColor = [SKColor colorWithRed:0.15 green:0.15 blue:0.3 alpha:1.0];
    
    [self createScenePhyiscsBorder];
    self.physicsWorld.contactDelegate = self;
    
    [self spawnProjectileAtPosition:CGPointMake(CGRectGetMidX(self.frame), 10)];
    
    [self spawnBricks];
}

#pragma mark -
#pragma mark PhysicsWorld Delegate Functions
-(void)didBeginContact:(SKPhysicsContact *)contact
{
    [self spawnContactExplosionAtLoc:contact.contactPoint];
}

#pragma mark -

-(void)spawnContactExplosionAtLoc:(CGPoint)loc
{
    SKEmitterNode *spark =  [NSKeyedUnarchiver unarchiveObjectWithFile:[[NSBundle mainBundle] pathForResource:@"contact_sparks" ofType:@"sks"]];
    
    spark.position = loc;
    
    //  This makes sure that the particle emitter is removed from the scene once it's don'e emitting, otherwise
    //  it will still be attached to the scene even if it's not emitting any particles.
    SKAction *cleanupSequence = [SKAction sequence:@[[SKAction waitForDuration:1.0], [SKAction removeFromParent]]];
    [spark runAction:cleanupSequence];
    
    [self addChild:spark];
}

-(void)createScenePhyiscsBorder
{
    self.physicsBody = [SKPhysicsBody bodyWithEdgeLoopFromRect:self.frame];
    self.physicsBody.categoryBitMask = bordersCategory;
    self.physicsBody.collisionBitMask = ballCategory | brickCategory;
    self.physicsBody.contactTestBitMask =  brickCategory | ballCategory;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    /* Called when a touch begins */
    
    for (UITouch *touch in touches) {
        CGPoint touchPoint = [touch locationInNode:self];
        CGPoint emitterPos = _emitter.position;
        CGVector vec = [self normalize: CGVectorMake(touchPoint.x - emitterPos.x, touchPoint.y - emitterPos.y)];
        [_emitter.physicsBody applyImpulse:vec];
    }
}

-(CGVector)normalize:(CGVector)inVector
{
    float length = sqrt(inVector.dx*inVector.dx + inVector.dy*inVector.dy);
    
    return CGVectorMake(inVector.dx/length, inVector.dy/length);
}

-(void)spawnProjectileAtPosition:(CGPoint)loc
{
    _emitter =  [NSKeyedUnarchiver unarchiveObjectWithFile:[[NSBundle mainBundle] pathForResource:@"ball_trail" ofType:@"sks"]];
    
    _emitter.position = loc;
    _emitter.name = @"exhaust";
    _emitter.targetNode = self.scene;
    
    _emitter.physicsBody = [SKPhysicsBody bodyWithCircleOfRadius:8];
    SKPhysicsBody *emitterPhysics = _emitter.physicsBody;
    emitterPhysics.dynamic = YES;
    emitterPhysics.angularDamping = 0.0;
    emitterPhysics.restitution = 1.0;
    emitterPhysics.linearDamping = 0.0;
    emitterPhysics.friction = 0.0;
    emitterPhysics.affectedByGravity = NO;
    emitterPhysics.categoryBitMask = ballCategory;
    emitterPhysics.collisionBitMask = brickCategory | bordersCategory;
    emitterPhysics.contactTestBitMask = brickCategory;
    
    [self addChild:_emitter];
}

-(void)spawnBricks
{
    CGFloat startY = CGRectGetMaxY(self.frame) - 30;
    CGFloat startX = CGRectGetMidX(self.frame);
    for(int i = 0; i < MAX_BRICKS; i++)
    {
        [self spawnBrickAtLocation:CGPointMake(startX, startY)];
        startY -= STD_BRICK.height + 30;
    }
}

-(void)spawnBrickAtLocation:(CGPoint)loc
{
    SKShapeNode* boxShape = [[SKShapeNode alloc] init];
    CGMutablePathRef myPath = CGPathCreateMutable();
    
    CGPathAddRect(myPath, NULL, BRICK_RECT(STD_BRICK.width, STD_BRICK.height));
    boxShape.path = myPath;
    boxShape.lineWidth = 0.1;
    boxShape.fillColor = [SKColor blueColor];
    boxShape.strokeColor = [SKColor redColor];
    
    boxShape.position = loc;
    boxShape.physicsBody = [SKPhysicsBody bodyWithRectangleOfSize:STD_BRICK];
    
    SKPhysicsBody *brickPhysics = boxShape.physicsBody;
    brickPhysics.dynamic = YES;
    brickPhysics.affectedByGravity = NO;
    brickPhysics.categoryBitMask = brickCategory;
    brickPhysics.collisionBitMask = ballCategory | brickCategory | bordersCategory;
    brickPhysics.contactTestBitMask = ballCategory | brickCategory;
    
    [self addChild: boxShape];
}

-(void)update:(CFTimeInterval)currentTime {
    /* Called before each frame is rendered */
}

@end
